#include "mainrun.h"

AMC::MainRun::MainRun(double rate, double gain, size_t frameSize, size_t N) :
    _rate(rate),
    _gain(gain),
    _frameSize(frameSize),
    _N(N),
    _isRunning(false),
    _runThread(),
    _runMode(AMC::RunMode::STOPPED),
    _classifierFileName(""),
    _dir(""),
    _buffer(),
    _fc(),
    _window(),
    _modType(),
    _featureExtractor(),
    _classifier(),
    _fftGenerator(),
    _qApp(),
    _mainWindow()
{

}

void AMC::MainRun::start(AMC::RunMode runMode,
                         AMC::ClassifierType classifierType,
                         std::string classifierFileName,
                         StreamFunction *streamFunc)
{
    switch(runMode)
    {
    case AMC::CLASSIFY:
        _dataStream.reset(new UhdMock(streamFunc, _rate, _gain, _frameSize));
        configurePrivateVars();
        setClassifier(classifierType);
        break;

    case AMC::CLASSIFY_USRP:
    case AMC::CAPTURE_DATA:
    case AMC::TRAIN_CLASSIFIER:
    case AMC::TEST_CLASSIFIER:
    case AMC::STOPPED:
        throw std::runtime_error("AMC::MainRun Error: Incorrect start conditions called.");
        break;
    }
}

void AMC::MainRun::start(AMC::RunMode runMode,
                         AMC::ClassifierType classifierType,
                         std::string classifierFileName)
{
    setClassifier(classifierType);

    switch(runMode)
    {
    case AMC::CLASSIFY_USRP:
        _dataStream.reset(new UhdRead(_rate, 0.0, _gain, _frameSize));
        configurePrivateVars();

        break;

    case AMC::TRAIN_CLASSIFIER:
    case AMC::TEST_CLASSIFIER:
    case AMC::CLASSIFY:
    case AMC::CLASSIFY_USRP:
    case AMC::CAPTURE_DATA:
    case AMC::STOPPED:
        throw std::runtime_error("AMC::MainRun Error: Incorrect start conditions called.");
        break;
    }
}

void AMC::MainRun::start(AMC::RunMode runMode,
                         ClassifierType classifierType,
                         std::string classifierFileName,
                         std::string dir)
{
    switch(runMode)
    {
    case AMC::TRAIN_CLASSIFIER:
        _classifier.reset(getClassifier(classifierType));
        break;

    case AMC::CAPTURE_DATA:
    case AMC::CLASSIFY:
    case AMC::CLASSIFY_USRP:
    case AMC::TEST_CLASSIFIER:
    case AMC::STOPPED:
        throw std::runtime_error("AMC::MainRun Error: Incorrect start conditions called.");
        break;
    }
}

void AMC::MainRun::start(AMC::RunMode runMode,
                         AMC::ClassifierType classifierType,
                         std::string classifierFileName,
                         std::string dir,
                         const AMC::MonteCarloArgs &args)
{
    setClassifier(classifierType);

    switch(runMode)
    {
    case AMC::CAPTURE_DATA:
        break;
    case AMC::TEST_CLASSIFIER:
        break;

    case AMC::CLASSIFY:
    case AMC::CLASSIFY_USRP:
    case AMC::TRAIN_CLASSIFIER:
    case AMC::STOPPED:
        throw std::runtime_error("AMC::MainRun Error: Incorrect start conditions called.");
        break;
    }
}

void AMC::MainRun::setClassifier(AMC::ClassifierType classifierType)
{
    _featureExtractor->setClassifier(getClassifier(classifierType));
}

void AMC::MainRun::setClassifier(AMC::ClassifierType classifierType, std::string classifierFileName)
{
    AmcClassifier<double, AMC::ModType> * classifier;
    classifier = getClassifier(classifierType);
    classifier->load(classifierFileName);
    _featureExtractor->setClassifier(classifier);
}

AmcClassifier<double, AMC::ModType> *AMC::MainRun::getClassifier(AMC::ClassifierType classifierType)
{
    switch(classifierType)
    {
    case (AMC::ClassifierType::CV_CLASSIFIER):
        return new AmcCvDecisionTree();
    case (AMC::ClassifierType::ZN_CLASSIFIER):
        return new AmcZnDecisionTree();
    }
}

void AMC::MainRun::configurePrivateVars()
{
    _buffer.swap(_dataStream->getBuffer());
    _fc.swap(_dataStream->getFc());
    _window.swap(_dataStream->getWindow());

    boost::unique_lock<boost::shared_mutex> fcLock(*_fc->getMutex());
    _fc->getData() = 0.1;
    fcLock.unlock();

    boost::unique_lock<boost::shared_mutex> winLock(*_window->getMutex());
    _window->getData() = 0.1;
    winLock.unlock();

    _featureExtractor.reset(new FeatureExtractor(
                                _buffer,
                                _rate,
                                _fc,
                                _window,
                                _N));

    _modType.swap(_featureExtractor->getSharedModType());

}

void AMC::MainRun::configureGuiVars()
{
    _fftGenerator.reset(new FFTGenerator(
                            _buffer,
                            _rate,
                            _N));

    _mainWindow.reset(new MainWindow(_rate, _N));
    _mainWindow->setData(_fftGenerator->getFreqVec(), _fftGenerator->getFftVec());
    _mainWindow->setBuffer(_buffer);
    _mainWindow->setSharedModType(_modType);
    _mainWindow->setFc(_fc);
    _mainWindow->setWindow(_window);
}

void AMC::MainRun::run()
{

}

