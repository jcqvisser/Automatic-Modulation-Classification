#include "featureextractor.h"

AMC::FeatureExtractor::FeatureExtractor(
        boost::shared_ptr<SharedBuffer<std::complex<double> > > buffer,
        size_t frameSize) :
    _buffer(buffer), _frameSize(frameSize)
{
}

void AMC::FeatureExtractor::run()
{
    auto bufferMutex = _buffer->getMutex();
    boost::shared_lock<boost::shared_mutex> bufferLock(*bufferMutex);
    while (_buffer->getBuffer().size() < _frameSize)
    {
        //TODO: How long to sleep?
        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
    }

    boost::unique_lock<boost::shared_mutex> dataLock(_dataMutex);
    for (size_t n = 0; n < _frameSize; ++n)
    {
        _data[n] = _buffer->getBuffer()[n];
    }
    bufferLock.release();
    dataLock.release();

    boost::thread sigmaAMu42A(findSigmaAMu42A);
    boost::thread mu42SigmaAF(findMu42FSigmaAF);

    sigmaAMu42A.join();
    mu42SigmaAF.join();
}

void AMC::FeatureExtractor::findSigmaAMu42A()
{
    boost::shared_lock<boost::shared_mutex> dataLock(_dataMutex);
    boost::shared_lock<boost::shared_mutex> dataNCLock(_dataNormCenterMutex);
    // make normed and centered copy
    dataLock.release();
    boost::thread sigmaAA(findSigmaAA);
    // find std dev and kurt
    dataNCLock.release();
    // save sigmaA and mu43A
    sigmaAA.join();
}

void AMC::FeatureExtractor::findMu42FSigmaAF()
{
    boost::shared_lock<boost::shared_mutex> dataLock(_dataMutex);
    boost::shared_lock<boost::shared_mutex> dataFftLock(_dataFftMutex);
    // do fft
    dataLock.release();
    boost::thread gammaMax(findGammaMax);
    boost::thread p(findP);
    // rmove - component
    dataFftLock.release();
    // do ifft
    // find phase
    // phase unwrap
    boost::thread sigmaDP(findSigmaDP);
    //differentiate
    //norm and center
    //kurt and stddev
    //save mu42f and sigmaaf
    gammaMax.join();
    p.join();
    sigmaDP.join();
}

void AMC::FeatureExtractor::findSigmaAA()
{
    boost::shared_lock<boost::shared_mutex> dataNCLock(_dataNormCenterMutex);
    // abs
    dataNCLock.release();
    // stdDev (combine abs with stdDev?)
    // save sigmaAA
}

void AMC::FeatureExtractor::findGammaMax()
{
    boost::shared_lock<boost::shared_mutex> dataFftLock(_dataFftMutex);
    // power
    dataFftLock.release();
    // max
    // save gammaMax
}

void AMC::FeatureExtractor::findP()
{
    boost::shared_lock<boost::shared_mutex> dataFftLock(_dataFftMutex);
    // compute spectral symmetry
    dataFftLock.release();
    // save p
}

void AMC::FeatureExtractor::findSigmaDP()
{
    boost::shared_lock<boost::shared_mutex> dataPhaseLock(_dataPhaseMutex);
    boost::unique_lock<boost::shared_mutex> dataPhaseNLWriteLock(_dataPhaseNLMutex);

    // remove linear phase

    dataPhaseLock.release();
    dataPhaseNLWriteLock.release();
    boost::shared_lock<boost::shared_mutex> dataPhaseNLReadLock(_dataPhaseNLMutex);

    boost::thread sigmaAP(findSigmaAP);

    // std dev

    dataPhaseNLReadLock.release();

    // save sigmadp

    sigmaAP.join();
}

void AMC::FeatureExtractor::findSigmaAP()
{
    boost::shared_lock<boost::shared_mutex> dataPhaseNLLock(_dataPhaseNLMutex);
    // take abs
    dataPhaseNLLock.release();
    // std dev
    // save sigmaAP
}
