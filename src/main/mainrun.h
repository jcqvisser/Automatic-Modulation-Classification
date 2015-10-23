#ifndef MAINRUN_H
#define MAINRUN_H

#include <QApplication>

#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include <exception>

#include "amc.h"
#include "streamer.h"
#include "uhdmock.h"
#include "uhdread.h"
#include "featureextractor.h"
#include "sharedbuffer.h"
#include "sharedtype.h"
#include "montecarlogen.h"
#include "fftgenerator.h"

#include "interface/mainwindow.h"

#include "classifier/amcclassifier.h"
#include "classifier/amccvdecisiontree.h"
#include "classifier/amczndescisiontree.h"

#include "modulators/streamfunction.h"

namespace AMC {

struct MinMax {
    double min;
    double max;
};

struct MonteCarloArgs {
    MinMax modIndex;
    MinMax fmModIndex;
    MinMax freq;
    MinMax digiFreq;
    MinMax constellationPow;
    MinMax snr;
    double timePerScheme;
};

enum RunMode{ CLASSIFY, CLASSIFY_USRP, CAPTURE_DATA, TRAIN_CLASSIFIER, TEST_CLASSIFIER, STOPPED };
enum ClassifierType{ ZN_CLASSIFIER, CV_CLASSIFIER };

class MainRun
{
public:
    MainRun(double rate, double gain, size_t frameSize, size_t N);

    void start(RunMode runMode);

    void start(RunMode runMode,
               ClassifierType classifierType,
               std::string classifierFileName,
               StreamFunction * streamFunc);

    void start(RunMode runMode,
               ClassifierType classifierType,
               std::string classifierFileName);

    void start(RunMode runMode,
               ClassifierType classifierType,
               std::string classifierFileName,
               std::string dir);

    void start(RunMode runMode,
               ClassifierType classifierType,
               std::string classifierFileName,
               const MonteCarloArgs &args);

    void stop();

private:
    void setClassifier(ClassifierType classifierType);
    void setClassifier(ClassifierType classifierType, std::string classifierFileName);
    AmcClassifier<double, AMC::ModType> *getClassifier(ClassifierType classifierType);
    void configurePrivateVars(); // Called after data streamer is created.
    void configureGuiVars();
    void run();

    double _rate;
    double _gain;
    size_t _frameSize;
    size_t _N;

    bool _isRunning;
    boost::thread _runThread;
    RunMode _runMode;

    std::string _classifierFileName;
    std::string _dir;

    boost::shared_ptr <SharedBuffer <std::complex <double> > > _buffer;
    boost::shared_ptr <SharedType <double> > _fc;
    boost::shared_ptr <SharedType <double> > _window;
    boost::shared_ptr <SharedType <AMC::ModType> _modType;

    boost::scoped_ptr <Streamer> _dataStream;
    boost::scoped_ptr <FeatureExtractor> _featureExtractor;
    boost::scoped_ptr <AmcClassifier <double,AMC::ModType> > _classifier;
    boost::scoped_ptr <FFTGenerator> _fftGenerator;
    boost::scoped_ptr <MonteCarloGen> _monteCarloGen;

    QApplication _qApp;
    boost::scoped_ptr <MainWindow> _mainWindow;
};

}


#endif // MAINRUN_H
