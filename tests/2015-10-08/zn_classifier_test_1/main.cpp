#include <QApplication>
#include <QMetaType>
#include <boost/smart_ptr.hpp>

#include "uhdmock.h"
//#include "uhdread.h"
#include "streamer.h"
#include "fftgenerator.h"
#include "sharedvector.h"
#include "amcrecv.h"
#include "featureextractor.h"

#include "interface/mainwindow.h"

//#include "classifier/amccvdecisiontree.h"
#include "classifier/amczndescisiontree.h"

#include "modulators/cosfunction.h"
#include "modulators/amfunction.h"
#include "modulators/fmfunction.h"
#include "modulators/digitalfunction.h"
#include "modulators/mpskfunction.h"
#include "modulators/awgnfunction.h"
#include "modulators/efunction.h"

#include "demodulators/amcdemodulator.h"
#include "demodulators/amdemod.h"
#include "demodulators/fmdemod.h"
#include "demodulators/digitaldemod.h"
#include "demodulators/mpskdemod.h"
#include "classifier/classifiertrainer.h"

int main(int argc, char *argv[])
{
    // Signal settings.
    double rate = 1e6;
    double freq = 10e3;
    double fc = 150e3;

    // Modulation settings
    double gain = 1;
    double rel_fc = fc / rate;
    double rel_fs = freq / rate;

    // Specific Modulation Settings.
    AmDemod::SideBand sideBand = AmDemod::SideBand::DOUBLE;
    unsigned int constSize = 16;
    int supp_carrier = 1;
    double mod_index = 0.5;

    // Frame size and FFT size.
    size_t N = 2048;
    size_t frameSize = 384;

/***************************************************************************************************
 *                                      Create stream object                                       *
 **************************************************************************************************/

    // AM Stream function.
    StreamFunction * _streamFunction = new AmFunction(new cosFunction(freq), mod_index, rel_fc, sideBand, supp_carrier);
    //StreamFunction * _streamFunction = new eFunction(freq);
    StreamFunction * _noisyStreamFunction = new AwgnFunction(_streamFunction, 0, 1e6, 1e6);
    // FM Stream Function
    //StreamFunction * _streamFunction = new FmFunction(new cosFunction(freq), mod_index, rel_fc);

    // MPSK Stream Function
    //StreamFunction * _streamFunction = new DigitalFunction(new MPskFunction(constSize), rel_fs, rel_fc);

    // ------------ Create Streamer Object ------------ //
    // UhdMock Object
    boost::scoped_ptr < Streamer > _dataStream(new UhdMock(_noisyStreamFunction, rate, gain, frameSize));
    // UhdRead Object
    //    boost::scoped_ptr < Streamer > _dataStream(new UhdRead(rate, freq, gain, frameSize));

/***************************************************************************************************
 *                                     Initialize utilities                                        *
 **************************************************************************************************/

    // Get shared buffer
    boost::shared_ptr < SharedBuffer<std::complex<double> > > _buffer(_dataStream->getBuffer());

    // Create fft generator function.
    FFTGenerator _fftGen(_buffer, rate, N);

    AmcClassifier<double, AMC::ModType> * classifier = new AmcZnDecisionTree();
    //classifier->load("test0");
    std::string dir = "/home/jcq/git/Automatic-Modulation-Classification-ELEN4012/train-data/2015-10-07";
    ClassifierTrainer ct(classifier,dir);
    ct.train();
    ct.save("test0");
    AMC::FeatureExtractor _featureExtractor(_buffer, classifier, N, rate);

/***************************************************************************************************
 *                                      Create Demodulator                                         *
 **************************************************************************************************/

    // Create demodulator object.
    AmcRecv _amcRecv(_buffer, N);

    // Am Demodulator.
        _amcRecv.setDemod(new AmDemod(mod_index, rel_fc, sideBand, supp_carrier));

    // Fm Demodulator.
//    _amcRecv.setDemod(new FmDemod(mod_index, rel_fc));

    // MPSK Demodulator
//    _amcRecv.setDemod(new DigitalDemod(new MPskDemod(constSize), rel_fs, rel_fc));

/***************************************************************************************************
 *                                      Initialize GUI Objects                                     *
 **************************************************************************************************/

    // Initialize interface.
    QApplication _app(argc, argv);
    MainWindow _mainWindow(rate);
    _mainWindow.show();

    _mainWindow.setData(_fftGen.getFreqVec(), _fftGen.getFftVec());
    _mainWindow.setBuffer(_buffer);
    _mainWindow.setSharedModType(_featureExtractor.getSharedModType());

/***************************************************************************************************
 *                                          Start threads.                                         *
 **************************************************************************************************/

    _dataStream->startStream();
//    _amcRecv.startDemod();
    _fftGen.startFft();
    _featureExtractor.start(AMC::FeatureExtractor::ExtractionMode::CLASSIFY);

    return _app.exec();
}
