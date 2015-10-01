#include "featureextractor.h"

AMC::FeatureExtractor::FeatureExtractor(
        boost::shared_ptr<SharedBuffer<std::complex<double> > > buffer,
        size_t frameSize, double fs) :
    _buffer(buffer), _frameSize(frameSize), _fs(fs)
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

    _xNormCenter.getData() = AMC::center(_x.getData());

    xLock.release();

    _xNormCenter.getData() = AMC::normalize(_xNormCenter.getData());

    xNormCenterWriteLock.release();
    boost::shared_lock<boost::shared_mutex> xNormCenterReadLock(*_xNormCenter.getMutex());
    boost::thread sigmaAA(&AMC::FeatureExtractor::findSigmaAA, this);

    AMC::stdDevKurtosis( _xNormCenter.getData(), _sigmaA, _mu42A);
    xNormCenterReadLock.release();
    sigmaAA.join();
}

void AMC::FeatureExtractor::findMu42FSigmaAF()
{
    boost::shared_lock<boost::shared_mutex> xLock(*_x.getMutex());
    boost::unique_lock<boost::shared_mutex> xFftWriteLock(*_xFft.getMutex());

    _xFft.getData() = AMC::fft(_x.getData());

    xLock.release();
    xFftWriteLock.release();
    boost::shared_lock<boost::shared_mutex> xFftReadLock(*_xFft.getMutex());
    boost::thread gammaMaxP(&AMC::FeatureExtractor::findGammaMaxP, this);

    auto xAnalFreq = AMC::removeNegFreq(_xFft.getData());

    xFftReadLock.release();

    auto xAnal = AMC::ifft(xAnalFreq);
    auto xWrappedPhase = AMC::phase(xAnal); //TODO
    _xPhase.getData() = AMC::unwrapPhase(xWrappedPhase);

    boost::shared_lock<boost::shared_mutex> xPhaseLock(*_xPhase.getMutex());
    boost::thread sigmaDP(&AMC::FeatureExtractor::findSigmaDP, this);

    std::vector<double> xRelativeInstFreq = AMC::differentiate(_xPhase.getData()); //linear shift when finding actual freq

    xPhaseLock.release();
    auto xRelativeInstFreqN = AMC::normalize(xRelativeInstFreq);
    std::vector<double> xRelativeInstFreqNormCenter = AMC::center(xRelativeInstFreqN);
    AMC::stdDevKurtosis(xRelativeInstFreqNormCenter, _mu42F, _sigmaAF);

    gammaMaxP.join();
    sigmaDP.join();
}

void AMC::FeatureExtractor::findSigmaAA()
{
    boost::shared_lock<boost::shared_mutex> xNormCenter(*_xNormCenter.getMutex());
    auto xNormCenterAbs = AMC::abs(_xNormCenter.getData());

    xNormCenter.release();

    _sigmaAA = AMC::stdDev(_xNormCenter.getData());
}

void AMC::FeatureExtractor::findGammaMaxP()
{
    boost::shared_lock<boost::shared_mutex> xFftLock(*_xFft.getMutex());

    _gammaMax = AMC::maxPower(_xFft.getData(),_fnc);
    _P = AMC::symmetry(_xFft.getData(), _fnc);

    xFftLock.release();
}

void AMC::FeatureExtractor::findSigmaDP()
{
    boost::shared_lock<boost::shared_mutex> xPhaseLock(*_xPhase.getMutex());
    boost::unique_lock<boost::shared_mutex> xPhaseNLWriteLock(*_xPhaseNL.getMutex());

    _xPhaseNL.getData() = AMC::removeLinearPhase(_xPhase.getData(), _fnc);

    xPhaseLock.release();
    xPhaseNLWriteLock.release();
    boost::shared_lock<boost::shared_mutex> xPhaseNLReadLock(*_xPhaseNL.getMutex());

    boost::thread sigmaAP(&AMC::FeatureExtractor::findSigmaAP, this);

    _sigmaDP = AMC::stdDev(_xPhaseNL.getData());

    xPhaseNLReadLock.release();

    sigmaAP.join();
}

void AMC::FeatureExtractor::findSigmaAP()
{
    boost::shared_lock<boost::shared_mutex> xPhaseNLLock(*_xPhaseNL.getMutex());

    std::vector<double> xAbsPhase = AMC::abs(_xPhaseNL.getData());

    xPhaseNLLock.release();

    _sigmaAP = AMC::stdDev(xAbsPhase);
}
