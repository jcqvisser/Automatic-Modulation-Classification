#include "featureextractor.h"

AMC::FeatureExtractor::FeatureExtractor(
        boost::shared_ptr<SharedBuffer<std::complex<double> > > buffer,
        size_t frameSize) :
    _buffer(buffer), _frameSize(frameSize)
{
}

void AMC::FeatureExtractor::run()
{
    boost::shared_lock<boost::shared_mutex> bufferLock(*_buffer->getMutex());
    while (_buffer->getBuffer().size() < _frameSize)
    {
        //TODO: How long to sleep?
        bufferLock.release();
        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
        bufferLock.lock();
    }

    boost::unique_lock<boost::shared_mutex> xWriteLock(*_x.getMutex());
    for (size_t n = 0; n < _frameSize; ++n)
    {
        _x.getData()[n] = _buffer->getBuffer()[n];
    }
    bufferLock.release();
    xWriteLock.release();

    boost::shared_lock<boost::shared_mutex> xReadLock(*_x.getMutex());

    boost::thread sigmaAMu42A(&AMC::FeatureExtractor::findSigmaAMu42A, this);
    boost::thread mu42FSigmaAF(&AMC::FeatureExtractor::findMu42FSigmaAF, this);

    sigmaAMu42A.join();
    mu42FSigmaAF.join();
}

void AMC::FeatureExtractor::findSigmaAMu42A()
{
    boost::shared_lock<boost::shared_mutex> xLock(*_x.getMutex());
    boost::unique_lock<boost::shared_mutex> xNormCenterWriteLock(*_xNormCenter.getMutex());
    // make normed and centered copy
    xLock.release();
    xNormCenterWriteLock.release();
    boost::shared_lock<boost::shared_mutex> xNormCenterReadLock(*_xNormCenter.getMutex());
    boost::thread sigmaAA(&AMC::FeatureExtractor::findSigmaAA, this);
    // find std dev and kurt
    xNormCenterReadLock.release();
    // save sigmaA and mu43A
    sigmaAA.join();
}

void AMC::FeatureExtractor::findMu42FSigmaAF()
{
    boost::shared_lock<boost::shared_mutex> xLock(*_x.getMutex());
    boost::unique_lock<boost::shared_mutex> xFftWriteLock(*_xFft.getMutex());
    // do fft
    xLock.release();
    xFftWriteLock.release();
    boost::shared_lock<boost::shared_mutex> xFftReadLock(*_xFft.getMutex());

    boost::thread gammaMax(&AMC::FeatureExtractor::findGammaMax, this);
    boost::thread p(&AMC::FeatureExtractor::findP, this);
    // rmove - component
    xFftReadLock.release();
    // do ifft
    // find phase
    // phase unwrap
    boost::shared_lock<boost::shared_mutex> xPhaseLock(*_xPhase.getMutex());
    boost::thread sigmaDP(&AMC::FeatureExtractor::findSigmaDP, this);
    //differentiate
    xPhaseLock.release();
    //norm and center
    //kurt and stddev
    //save mu42f and sigmaaf
    gammaMax.join();
    p.join();
    sigmaDP.join();
}

void AMC::FeatureExtractor::findSigmaAA()
{
    boost::shared_lock<boost::shared_mutex> xNormCenter(*_xNormCenter.getMutex());
    // abs
    xNormCenter.release();
    // stdDev (combine abs with stdDev?)
    // save sigmaAA
}

void AMC::FeatureExtractor::findGammaMax()
{
    boost::shared_lock<boost::shared_mutex> xFftLock(*_xFft.getMutex());
    // power
    xFftLock.release();
    // max
    // save gammaMax
}

void AMC::FeatureExtractor::findP()
{
    boost::shared_lock<boost::shared_mutex> xFftLock(*_xFft.getMutex());
    // compute spectral symmetry
    xFftLock.release();
    // save p
}

void AMC::FeatureExtractor::findSigmaDP()
{
    boost::shared_lock<boost::shared_mutex> xPhaseLock(*_xPhase.getMutex());
    boost::unique_lock<boost::shared_mutex> xPhaseNLWriteLock(*_xPhaseNL.getMutex());

    // remove linear phase

    xPhaseLock.release();
    xPhaseNLWriteLock.release();
    boost::shared_lock<boost::shared_mutex> xPhaseNLReadLock(*_xPhaseNL.getMutex());

    boost::thread sigmaAP(&AMC::FeatureExtractor::findSigmaAP, this);

    // std dev

    xPhaseNLReadLock.release();

    // save sigmadp

    sigmaAP.join();
}

void AMC::FeatureExtractor::findSigmaAP()
{
    boost::shared_lock<boost::shared_mutex> xPhaseNLLock(*_xPhaseNL.getMutex());
    // take abs
    xPhaseNLLock.release();
    // std dev
    // save sigmaAP
}
