#ifndef DATACOLORMODEL_H
#define DATACOLORMODEL_H

#include "NaLockableData.h"
#include "NaVolumeData.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <qrgb.h>

// DataColorModel converts multichannel 16-bit intensities
// into rgb colors by combining:
//  1 - a color for each data channel
//  2 - min and max "HDR" limits for each channel
//  3 - gamma brightness correction for each channel
// TODO - deprecate BrightnessCalibrator for DataColorModel
// TODO - support both blend-SUM and blend-MAX
// TODO - get (initial) hdr range from input volume
// TODO - allow custom hdr for gallery thumbnails (where does that even go?)
class DataColorModel : public NaLockableData
{
    Q_OBJECT

public:
    explicit DataColorModel(
            const NaVolumeData& volumeDataParam,
            QObject *parentParam = NULL);

public slots:
    void resetColors();
    void setChannelColor(int index, QRgb color);
    void setChannelHdrRange(int index, qreal min, qreal max);
    void setChannelGamma(int index, qreal gamma);
    void setGamma(qreal gamma); // all channels

public:
    // ChannelColorModel specifies the colorization parameters for a single data channel
    class ChannelColorModel
    {
    public:
        friend class DataColorModel;

        ChannelColorModel(QRgb channelColorParam)
            : blackColor(qRgb(0, 0, 0))
        {
            setColor(channelColorParam);
            setGamma(1.0f);
            setHdrRange(0.0f, 4095.0f);
        }

        void setColor(QRgb channelColorParam)
        {
            channelColor = channelColorParam;
            colorRed = qRed(channelColor);
            colorGreen = qGreen(channelColor);
            colorBlue = qBlue(channelColor);
        }

        QRgb getColor() const {return channelColor;}

        void setHdrRange(qreal hdrMinParam, qreal hdrMaxParam)
        {
            hdrMin = hdrMinParam;
            hdrMax = hdrMaxParam;
            hdrRange = std::max(qreal(1.0), hdrMax - hdrMin);
        }

        void setGamma(qreal gammaParam)
        {
            gamma = gammaParam;
            gammaIsNotUnity = (gamma != 1.0f); // premature optimization
            // populate gamma lookup table
            qreal previous_i_out = 0.0f;
            for (int i = 0; i < 256; ++i) {
                qreal i_in = i / 255.0f; // range 0.0-1.0
                qreal i_out = std::pow(i_in, gamma);
                gammaTable[i] = i_out;
                if (i > 0)
                    dGammaTable[i - 1] = i_out - previous_i_out;
                previous_i_out = i_out;
            }
            dGammaTable[255] = 0.0f; // final unused entry for neatness
        }

        // getColor() definition is in header file so it can be inlined
        QRgb getColor(qreal intensity) const
        {
            // 1) Apply hdr interval
            qreal i = (intensity - hdrMin)/hdrRange;
            if (i <= 0.0) return blackColor; // clamp below
            if (i >= 1.0) return channelColor; // clamp above
            // 2) Apply gamma correction
            if (gammaIsNotUnity)
            {
               qreal gf = i * 255.0f; // index for lookup table, including decimal fraction
               int gi = int(gf); // index for lookup table
               i = gammaTable[gi] + dGammaTable[gi] * (gf - gi); // Taylor series interpolation
            }
            // 3) scale channel color
            return qRgb(int(i * colorRed), int(i * colorGreen), int(i * colorBlue));
        }

    protected:
        QRgb blackColor;
        QRgb channelColor; // color of channel
        int colorRed; // red component of channelColor, for efficiency
        int colorGreen;
        int colorBlue;
        qreal hdrMin; // minimum intensity to scale
        qreal hdrMax; // maximum intensity to scale
        qreal hdrRange; // hdrMax - hdrMin
        qreal gamma; // exponential brightness correction
        bool gammaIsNotUnity; // precompute gamma != 1.0
        // lookup table for faster gamma transform
        qreal gammaTable[256]; // cache for efficiency
        qreal dGammaTable[256]; // first derivative of gammaTable values
    };


    class Reader; friend class Reader;
    class Reader : public BaseReadLocker
    {
    public:
        Reader(const DataColorModel& colorModelParam)
            : BaseReadLocker(colorModelParam.getLock())
            , colorModel(colorModelParam)
        {}

        // TODO - create more accessor methods for downstream clients
        int getNumberOfDataChannels() const {
            return colorModel.channelColors.size(); // +1 for reference
        }

        QRgb blend(double channelIntensities[]) const {
            int red, green, blue;
            red = green = blue = 0;
            int sc = colorModel.channelColors.size();
            for (int c = 0; c < sc; ++c) {
                if (channelIntensities[c] == 0.0)
                    continue;
                const ChannelColorModel& ccm = colorModel.channelColors[c];
                QRgb channelColor = ccm.getColor(channelIntensities[c]);
                red += qRed(channelColor);
                green += qGreen(channelColor);
                blue += qBlue(channelColor);
            }
            red = std::min(255, red);
            green = std::min(255, green);
            blue = std::min(255, blue);
            return qRgb(red, green, blue);
        }

    private:
        const DataColorModel& colorModel;
    };


    class Writer : public QWriteLocker
    {
    public:
        Writer(DataColorModel& colorModelParam)
            : QWriteLocker(colorModelParam.getLock())
            , colorModel(colorModelParam)
        {}

    protected:
        DataColorModel& colorModel;
    };


protected:
    const NaVolumeData& volumeData;
    std::vector<ChannelColorModel> channelColors;
};

#endif // DATACOLORMODEL_H
