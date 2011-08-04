#include "MipFragmentData.h"

/* explicit */
MipFragmentData::MipFragmentData(const NaVolumeData& volumeDataParam, QObject *parent /* = 0 */)
    : NaLockableData(parent)
    , volumeData(volumeDataParam)
    , fragmentData(NULL)
    , fragmentZValues(NULL)
    , fragmentIntensities(NULL)
{
}

/* virtual */
MipFragmentData::~MipFragmentData()
{
    Writer mipWriter(*this); // wait for readers to release lock
    mipWriter.clearImageData();
}

/* slot */
void MipFragmentData::updateFromVolumeData()
{
    // acquire read lock on volume data
    NaVolumeData::Reader volumeReader(volumeData);
    if (! volumeReader.hasReadLock()) return; // Don't worry; we'll get another data push later.

    // acquire write lock on Mip data
    Writer mipWriter(*this);
    // perhaps it took some time to acquire the write lock, so check the volume again
    if (! volumeReader.refreshLock()) return;

    const Image4DProxy<My4DImage> originalProxy = volumeReader.getOriginalImageProxy();
    const Image4DProxy<My4DImage> referenceProxy = volumeReader.getReferenceImageProxy();
    const Image4DProxy<My4DImage> maskProxy = volumeReader.getNeuronMaskProxy();

    // Allocate mip images
    mipWriter.clearImageData();
    // allocate channel data
    fragmentData = new My4DImage();
    fragmentData->loadImage(
            originalProxy.sx,
            originalProxy.sy,
            volumeReader.getNumberOfNeurons() + 1, // frags + bkgd; (reference/nc82 channel is stored in fragmentIntensities)
            originalProxy.sc,
            volumeReader.getOriginalDatatype() );
    // set to zero
    memset(fragmentData->getRawData(), 0, fragmentData->getTotalBytes());
    if (! volumeReader.refreshLock()) return;

    // allocate Z buffer
    fragmentZValues = new My4DImage();
    fragmentZValues->loadImage(
            originalProxy.sx,
            originalProxy.sy,
            volumeReader.getNumberOfNeurons() + 2, // +1 reference/nc82 channel
            1, // only one channel, containing z values
            V3D_UINT16 );
    // clear each byte to xFF, should result in -1?
    memset(fragmentZValues->getRawData(), 255, fragmentZValues->getTotalBytes());
    if (! volumeReader.refreshLock()) return;

    // allocate intensity cache
    fragmentIntensities = new My4DImage();
    fragmentIntensities->loadImage(
            originalProxy.sx,
            originalProxy.sy,
            volumeReader.getNumberOfNeurons() + 2, // +1 reference/nc82 channel is stored in slice nFrags+1
            1, // only one channel, containing z intensities
            V3D_UINT16 );
    // initialize to zero
    memset(fragmentIntensities->getRawData(), 0, fragmentIntensities->getTotalBytes());
    if (! volumeReader.refreshLock()) return;

    Image4DProxy<My4DImage> mipProxy(fragmentData);
    Image4DProxy<My4DImage> zProxy(fragmentZValues);
    Image4DProxy<My4DImage> intensityProxy(fragmentIntensities);

    // Populate mip images
    int imageX = originalProxy.sx;
    int imageY = originalProxy.sy;
    int imageZ = originalProxy.sz;
    int imageC = originalProxy.sc;
    // Reference/nc82 channel appears after all of the neuron/fragments
    int refIndex = volumeReader.getNumberOfNeurons() + 1;
    for (int z = 0; z < imageZ; z++)
    {
        for (int y = 0; y < imageY; y++)
        {
            for (int x = 0; x < imageX; x++)
            {
                // Reference/nc82
                float referenceIntensity = referenceProxy.value_at(x,y,z,0);
                float previousReferenceIntensity = intensityProxy.value_at(x, y, refIndex, 0);
                if (referenceIntensity > previousReferenceIntensity) {
                    intensityProxy.put_at(x, y, refIndex, 0, referenceIntensity);
                    zProxy.put_at(x, y, refIndex, 0, z);
                }

                // Neurons and Background
                unsigned char maskIndex = maskProxy.value8bit_at(x,y,z,0);
                float previousIntensity = intensityProxy.value_at(x, y, maskIndex, 0);
                float intensity = 0;
                for (int c = 0; c < imageC; c++)
                    intensity += originalProxy.value_at(x, y, z, c);
                if (intensity > previousIntensity) {
                    intensityProxy.put_at(x, y, maskIndex, 0, intensity);
                    zProxy.put_at(x, y, maskIndex, 0, z);
                    for (int c = 0; c < imageC; c++)
                        mipProxy.put_at(x, y, maskIndex, c, originalProxy.value_at(x, y, z, c));
                }
            }
        }
        if (! volumeReader.refreshLock()) return; // Try to reacquire lock every 25 ms
    }

    // unlock before emitting
    volumeReader.unlock();
    mipWriter.unlock();
    emit dataChanged(); // success!
}


/////////////////////////////////////
// MipFragmentData::Writer methods //
/////////////////////////////////////

void MipFragmentData::Writer::clearImageData()
{
    m_mipFragmentData.fragmentMinimumIntensities.clear();
    m_mipFragmentData.fragmentMaximumIntensities.clear();

    if (m_mipFragmentData.fragmentData != NULL) {
        delete m_mipFragmentData.fragmentData;
        m_mipFragmentData.fragmentData = NULL;
    }
    if (m_mipFragmentData.fragmentZValues != NULL) {
        delete m_mipFragmentData.fragmentZValues;
        m_mipFragmentData.fragmentZValues = NULL;
    }
    if (m_mipFragmentData.fragmentIntensities != NULL) {
        delete m_mipFragmentData.fragmentIntensities;
        m_mipFragmentData.fragmentIntensities = NULL;
    }
}

