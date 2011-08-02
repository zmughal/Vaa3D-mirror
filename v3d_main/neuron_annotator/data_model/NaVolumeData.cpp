#include "NaVolumeData.h"
#include <iostream>

using namespace std;

NaVolumeData::NaVolumeData()
    : originalImageStack(NULL)
    , neuronMaskStack(NULL)
    , referenceStack(NULL)
{
}

NaVolumeData::~NaVolumeData()
{
    if (originalImageStack != NULL) {
        delete originalImageStack;
        originalImageStack = NULL;
    }
    if (neuronMaskStack != NULL) {
        delete neuronMaskStack;
        neuronMaskStack = NULL;
    }
    if (referenceStack != NULL) {
        delete referenceStack;
        referenceStack = NULL;
    }
}

bool NaVolumeData::loadOriginalImageStack(QString originalImageStackFilePath)
{
    QString msgPrefix("NaVolumeData::loadOriginalImageStack()");
    originalImageStack = new My4DImage;
    if (!originalImageStack) {
        cerr << msgPrefix.toStdString() << " : problem creating My4DImage" << endl;
        return false;
    }
    originalImageStack->loadImage(originalImageStackFilePath.toAscii().data());
    if (originalImageStack->isEmpty()) {
        cerr << msgPrefix.toStdString() << ": originalImageStack is empty after loading\n";
        return false;
    }
    cout << "Loaded original image stack with dimensions X=" << originalImageStack->getXDim() << " Y=" << originalImageStack->getYDim()
            << " Z=" << originalImageStack->getZDim() << " C=" << originalImageStack->getCDim() << "\n";
    return true;
}

bool NaVolumeData::loadNeuronMaskStack(QString maskLabelFilePath) {
    QString msgPrefix("NaVolumeData::loadNeuronMaskStack()");
    if (originalImageStack==0) {
        cerr << msgPrefix.toStdString() << " error : originalImageStack must be created before this function is called" << endl;
        return false;
    }
    neuronMaskStack = new My4DImage;
    if (!neuronMaskStack) {
        cerr << msgPrefix.toStdString() << " : problem creating My4DImage" << endl;
        return false;
    }
    neuronMaskStack->loadImage(maskLabelFilePath.toAscii().data());
    return true;
}

bool NaVolumeData::loadReferenceStack(QString referenceStackFilePath)
{
    // Phase 1: load the data
    QString msgPrefix("NaVolumeData::loadReferenceStack()");
    qDebug() << msgPrefix << " : start";
    My4DImage* initialReferenceStack=new My4DImage();
    initialReferenceStack->loadImage(referenceStackFilePath.toAscii().data());
    if (initialReferenceStack->isEmpty()) {
        cerr << msgPrefix.toStdString() << ": initialReferenceStack is empty after loading\n";
        return false;
    }
    cout << "Loaded reference stack stack with dimensions X=" << initialReferenceStack->getXDim() << " Y=" << initialReferenceStack->getYDim()
            << " Z=" << initialReferenceStack->getZDim() << " C=" << initialReferenceStack->getCDim() << "\n";

    // Phase 2: normalize to 8-bit
    referenceStack=new My4DImage();
    referenceStack->loadImage(initialReferenceStack->getXDim(), initialReferenceStack->getYDim(), initialReferenceStack->getZDim(), 1 /* number of channels */, 1 /* bytes per channel */);
    Image4DProxy<My4DImage> initialProxy(initialReferenceStack);
    Image4DProxy<My4DImage> referenceProxy(referenceStack);

    double initialMin=initialReferenceStack->getChannalMinIntensity(0);
    double initialMax=initialReferenceStack->getChannalMaxIntensity(0);

    qDebug() << "Populating reference with initial data";
    double initialRange=initialMax-initialMin;
    qDebug() << "Reference lsm initialMin=" << initialMin << " initialMax=" << initialMax << " initialRange=" << initialRange;
    int zDim=initialReferenceStack->getZDim();
    int yDim=initialReferenceStack->getYDim();
    int xDim=initialReferenceStack->getXDim();
    for (int z=0;z<zDim;z++) {
        for (int y=0;y<yDim;y++) {
            for (int x=0;x<xDim;x++) {
                int value= (255.0*(*initialProxy.at_uint16(x,y,z,0))-initialMin)/initialRange;
                if (value<0) {
                    value=0;
                } else if (value>255) {
                    value=255;
                }
                referenceProxy.put8bit_fit_at(x,(yDim-y)-1,z,0,value); // For some reason, the Y-dim seems to need inversion
            }
        }
    }
    initialReferenceStack->cleanExistData();
    delete initialReferenceStack;
    qDebug() << "Finished loading reference stack";

    return true;
}


//////////////////////////////////
// NaVolumeData::Reader methods //
//////////////////////////////////

const Image4DProxy<My4DImage> NaVolumeData::Reader::getNeuronMaskProxy()
{
    // Image4DProxy class is a bit const-retarded, so const_cast here.
    // (It's OK; we are returning a const object in the end.)
    return Image4DProxy<My4DImage>(const_cast<My4DImage*>(neuronMaskStack));
}

const Image4DProxy<My4DImage> NaVolumeData::Reader::getOriginalImageProxy()
{
    // Image4DProxy class is a bit const-retarded, so const_cast here.
    // (It's OK; we are returning a const object in the end.)
    return Image4DProxy<My4DImage>(const_cast<My4DImage*>(originalImageStack));
}

const Image4DProxy<My4DImage> NaVolumeData::Reader::getReferenceImageProxy()
{
    // Image4DProxy class is a bit const-retarded, so const_cast here.
    // (It's OK; we are returning a const object in the end.)
    return Image4DProxy<My4DImage>(const_cast<My4DImage*>(referenceStack));
}

