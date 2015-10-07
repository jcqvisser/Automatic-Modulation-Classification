#include "featureextractor.h"

AMC::FeatureExtractor::FeatureExtractor(
        boost::shared_ptr<SharedBuffer<std::complex<double> > > buffer,
        size_t windowSize, double fs) :
    _buffer(buffer),  _x(windowSize), _windowSize(windowSize), _fs(fs)
{
}

void AMC::FeatureExtractor::start(ExtractionMode mode)
{
    _isExtracting = true;
    _mode = mode;
    _extractorThread = boost::thread(&FeatureExtractor::run, this);
}

void AMC::FeatureExtractor::start(ExtractionMode mode, AMC::ModType modType)
{

    if (mode == WRITE_TO_FILE)
    {
        _mode = mode;
        _fileWriter.newFile(modType);
        _isExtracting = true;
        _extractorThread = boost::thread(&FeatureExtractor::run, this);
    }
}

void AMC::FeatureExtractor::stop()
{
    _mode = AMC::FeatureExtractor::ExtractionMode::STOPPED;
    _isExtracting = false;
    _extractorThread.join();
}

void AMC::FeatureExtractor::run()
{
    while (_isExtracting)
    {
        while (_buffer->getBuffer().size() < _windowSize)
        {
            //TODO: How long to sleep?
            boost::this_thread::sleep(boost::posix_time::milliseconds(10));
        }
        boost::shared_lock<boost::shared_mutex> bufferLock(*_buffer->getMutex());
        boost::unique_lock<boost::shared_mutex> xWriteLock(*_x.getMutex());

        for (size_t n = 0; n < _windowSize; ++n)
        {
            _x.getData()[n] = _buffer->getBuffer()[n];
        }

        bufferLock.unlock();
        xWriteLock.unlock();

        boost::thread sigmaAMu42A(&AMC::FeatureExtractor::findSigmaAMu42A, this);
        AMC::FeatureExtractor::findMu42FSigmaAF();

        sigmaAMu42A.join();
        switch(_mode)
        {
            case AMC::FeatureExtractor::WRITE_TO_FILE:
                _fileWriter.writeToFile(AMC::FeatureExtractor::getFeatureVector());
        }
    }
}

void AMC::FeatureExtractor::findSigmaAMu42A()
{
    boost::shared_lock<boost::shared_mutex> xLock(*_x.getMutex());
    boost::unique_lock<boost::shared_mutex> xNormCenterWriteLock(*_xNormCenter.getMutex());

    _xNormCenter.getData() = AMC::normalize(AMC::center(_x.getData()));

    xLock.unlock();
    xNormCenterWriteLock.unlock();

    boost::shared_lock<boost::shared_mutex> xNormCenterReadLock(*_xNormCenter.getMutex());
    boost::thread sigmaAA(&AMC::FeatureExtractor::findSigmaAA, this);

    AMC::stdDevKurtosis( _xNormCenter.getData(), _sigmaA, _mu42A);
    xNormCenterReadLock.unlock();

    sigmaAA.join();
}

void AMC::FeatureExtractor::findMu42FSigmaAF()
{
    boost::shared_lock<boost::shared_mutex> xLock(*_x.getMutex());
    boost::unique_lock<boost::shared_mutex> xFftWriteLock(*_xFft.getMutex());

    _xFft.getData() = AMC::fft(_x.getData());
    xLock.unlock();
    xFftWriteLock.unlock();

    boost::shared_lock<boost::shared_mutex> xFftReadLock(*_xFft.getMutex());

    boost::thread gammaMaxP(&AMC::FeatureExtractor::findGammaMaxP, this);

    auto xAnalFreq = AMC::removeNegFreq(_xFft.getData());
    xFftReadLock.unlock();

    auto xAnal = AMC::ifft(xAnalFreq);

    auto xWrappedPhase = AMC::phase(xAnal);

    boost::unique_lock<boost::shared_mutex> xPhaseWriteLock(*_xPhase.getMutex());
    _xPhase.getData() = AMC::unwrapPhase(xWrappedPhase);
    xPhaseWriteLock.unlock();

    boost::thread sigmaDP(&AMC::FeatureExtractor::findSigmaDP, this);

    boost::shared_lock<boost::shared_mutex> xPhaseLock(*_xPhase.getMutex());
    std::vector<double> xRelativeInstFreq = AMC::differentiate(_xPhase.getData()); //linear shift when finding actual freq
    xPhaseLock.unlock();

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
    xNormCenter.unlock();

    _sigmaAA = AMC::stdDev(xNormCenterAbs);
}

void AMC::FeatureExtractor::findGammaMaxP()
{
    boost::shared_lock<boost::shared_mutex> xFftLock(*_xFft.getMutex());

    _gammaMax = AMC::maxPower(_xFft.getData(),_fnc);
    _P = AMC::symmetry(_xFft.getData(), _fnc);
    xFftLock.unlock();
}

void AMC::FeatureExtractor::findSigmaDP()
{
    boost::shared_lock<boost::shared_mutex> xPhaseLock(*_xPhase.getMutex());
    boost::unique_lock<boost::shared_mutex> xPhaseNLWriteLock(*_xPhaseNL.getMutex());

    _xPhaseNL.getData() = AMC::removeLinearPhase(_xPhase.getData(), _fnc);
    xPhaseLock.unlock();
    xPhaseNLWriteLock.unlock();

    boost::thread sigmaAP(&AMC::FeatureExtractor::findSigmaAP, this);

    boost::shared_lock<boost::shared_mutex> xPhaseNLReadLock(*_xPhaseNL.getMutex());
    _sigmaDP = AMC::stdDev(_xPhaseNL.getData());
    xPhaseNLReadLock.unlock();

    sigmaAP.join();
}

void AMC::FeatureExtractor::findSigmaAP()
{
    boost::shared_lock<boost::shared_mutex> xPhaseNLLock(*_xPhaseNL.getMutex());
    std::vector<double> xAbsPhase = AMC::abs(_xPhaseNL.getData());
    xPhaseNLLock.unlock();

    _sigmaAP = AMC::stdDev(xAbsPhase);
}

std::vector<double> AMC::FeatureExtractor::getFeatureVector()
{
    std::vector<double> f = {_mu42F, _sigmaAF, _sigmaDP, _sigmaAP, _gammaMax, _P, _sigmaA, _mu42A, _sigmaAA};
    return f;
}


