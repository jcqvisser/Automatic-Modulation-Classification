#include "featureextractor.h"

AMC::FeatureExtractor::FeatureExtractor(boost::shared_ptr<SharedBuffer> buffer, size_t windowSize) :
    _buffer(buffer), _windowSize(windowSize)
{
}

void AMC::FeatureExtractor::run()
{
    boost::thread sigmaAMu42A(findSigmaAMu42A);
    boost::thread mu42SigmaAF(findMu42FSigmaAF);

    sigmaAMu42A.join();
    mu42SigmaAF.join();
}

void AMC::FeatureExtractor::findSigmaAMu42A()
{
    // get read lock on signal data
    // make normed and centered copy
    // release lock
    boost::thread sigmaAA(findSigmaAA);
    // find std dev and kurt
    // save sigmaA and mu43A
    sigmaAA.join();
}

void AMC::FeatureExtractor::findMu42FSigmaAF()
{
    // get read lock on signal data
    // do fft
    // release lock
    boost::thread gammaMax(findGammaMax);
    boost::thread p(findP);
    // rmove - component
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
    // get read lock on norm and centered original signal
    // abs
    // release lock
    // stdDev (combine abs with stdDev?)
    // save sigmaAA
}

void AMC::FeatureExtractor::findGammaMax()
{
    // get read lock on fft'ed signal
    // power
    // release lock
    // max
    // save gammaMax
}

void AMC::FeatureExtractor::findP()
{
    // get read lock on fft'ed signal
    // compute spectral symmetry
    // release lock
    // save p
}

void AMC::FeatureExtractor::findSigmaDP()
{
    // remove linear phase
    // get read lock on data with lin phase removed
    boost::thread sigmaAP(findSigmaAP);
    // std dev
    // release read lock
    // save sigmadp
    sigmaAP.join();
}

void AMC::FeatureExtractor::findSigmaAP()
{
    // get read lock on signal non linear phase
    // take abs
    // release read lock
    // std dev
    // save sigmaAP
}
