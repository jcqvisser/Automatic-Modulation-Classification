#include <QApplication>
#include <QMetaType>
#include <boost/smart_ptr.hpp>

#include "uhdmock.h"
#include "uhdread.h"
#include "streamer.h"
#include "fftgenerator.h"
#include "sharedvector.h"
#include "amcrecv.h"
#include "featureextractor.h"
#include "sharedtype.h"

#include "interface/mainwindow.h"

#include "classifier/amccvdecisiontree.h"
#include "classifier/amczndescisiontree.h"

#include "modulators/cosfunction.h"
#include "modulators/amfunction.h"
#include "modulators/fmfunction.h"
#include "modulators/digitalfunction.h"
#include "modulators/mpskfunction.h"
#include "modulators/maskfunction.h"
#include "modulators/mqamfunction.h"
#include "modulators/awgnfunction.h"
#include "modulators/multifunction.h"

#include "demodulators/amcdemodulator.h"
#include "demodulators/amdemod.h"
#include "demodulators/fmdemod.h"
#include "demodulators/digitaldemod.h"
#include "demodulators/mpskdemod.h"
#include "demodulators/maskdemod.h"

int main(int argc, char *argv[])
{
    // Signal settings.
    double rate = 2e6;
    // Frequency of transmitted signal
    double freq = 15e3;
    // Center frequency for window to start at.
    double fc = 100e3;

    // Modulation settings
    double gain = 1;
    double rel_fc = fc / rate;
    AmDemod::SideBand sideBand;
    int suppCarrier;

    // AM.
    double mod_index = 1;
    // FM
    double fm_mod_index = 25e3/rate;
    // Noise
    double snr = 20.0;
    // Digital
    double fs;
    unsigned int constSize;

    // Frame size and FFT size.
    size_t N = 4096;
    size_t frameSize = 384;

/***************************************************************************************************
 *                                      Create stream object                                       *
 **************************************************************************************************/

    std::vector< boost::shared_ptr <StreamFunction> > _streamFunctions;

    StreamFunction * _streamFunction;

    // Signal 1.
    double fc1 = rel_fc;
    sideBand = AmDemod::SideBand::DOUBLE;
    suppCarrier = 1;
    _streamFunction = new AmFunction(new cosFunction(freq), mod_index, fc1, sideBand, suppCarrier);
    _streamFunctions.push_back(boost::shared_ptr<StreamFunction>(_streamFunction));

    // Signal 2.
    double fc2 = rel_fc * 2;
    sideBand = AmDemod::SideBand::DOUBLE;
    suppCarrier =0;
    _streamFunction = new AmFunction(new cosFunction(freq), mod_index, fc2, sideBand, suppCarrier);
    _streamFunctions.push_back(boost::shared_ptr<StreamFunction>(_streamFunction));

    // Signal 3.
    double fc3 = rel_fc * 3;
    sideBand = AmDemod::SideBand::LOWER;
    suppCarrier = 1;
    _streamFunction = new AmFunction(new cosFunction(freq), mod_index, fc3, sideBand, suppCarrier);
    _streamFunctions.push_back(boost::shared_ptr<StreamFunction>(_streamFunction));

    // Signal 4.
    double fc4 = rel_fc * 4;
    sideBand = AmDemod::SideBand::UPPER;
    suppCarrier = 1;
    _streamFunction = new AmFunction(new cosFunction(freq), mod_index, fc4, sideBand, suppCarrier);
    _streamFunctions.push_back(boost::shared_ptr<StreamFunction>(_streamFunction));

    // Signal 5.
    double fc5 = rel_fc * 5;
    _streamFunction = new FmFunction(new cosFunction(freq), fm_mod_index, fc5);
    _streamFunctions.push_back(boost::shared_ptr<StreamFunction>(_streamFunction));

    // Signal 6.
    double fc6 = rel_fc * 6;
    fs = 20e3 / rate;
    constSize = 4;
    _streamFunction = new DigitalFunction(new MPskFunction(constSize), fs, fc6);
    _streamFunctions.push_back(boost::shared_ptr<StreamFunction>(_streamFunction));


    // ------------ Create Streamer Object ------------ //
    StreamFunction * _multiStreamFunction = new MultiFunction(_streamFunctions);

    StreamFunction * _awgnFunction = new AwgnFunction(_multiStreamFunction, snr, rate, 1e6);
    // UhdMock Object
    boost::scoped_ptr < Streamer > _dataStream(new UhdMock(_awgnFunction, rate, gain, frameSize));
    // UhdRead Object
//    boost::scoped_ptr < Streamer > _dataStream(new UhdRead(rate, freq, gain, frameSize));

/***************************************************************************************************
 *                                     Initialize utilities                                        *
 **************************************************************************************************/

    // Get shared buffer
    boost::shared_ptr < SharedBuffer<std::complex<double> > > _buffer(_dataStream->getBuffer());
    boost::shared_ptr < SharedType<double> > _fc_rel(_dataStream->getFc());
    boost::shared_ptr < SharedType<double> > _window(_dataStream->getWindow());

    _fc_rel->getData() = rel_fc;
    _window->getData() = 100e3 / rate;

    // Create fft generator function.
    FFTGenerator _fftGen(_buffer, rate, N);

    AmcClassifier<double, AMC::ModType> * classifier = new AmcCvDecisionTree();
//    AmcClassifier<double, AMC::ModType> * classifier = new AmcZnDecisionTree();
    AMC::FeatureExtractor _featureExtractor(_buffer, classifier, rate, _fc_rel, _window, N);

    boost::shared_ptr < SharedType<AMC::ModType> > _modType(_featureExtractor.getSharedModType());

/***************************************************************************************************
 *                                      Create Demodulator                                         *
 **************************************************************************************************/

    // Create demodulator object.
    AmcRecv _amcRecv(_buffer, rate, N);
    _amcRecv.setFc(_fc_rel);
    _amcRecv.setModType(_modType);

/***************************************************************************************************
 *                                      Initialize GUI Objects                                     *
 **************************************************************************************************/

    // Initialize interface.
    QApplication _app(argc, argv);
    MainWindow _mainWindow(rate, N);
    _mainWindow.show();

/***************************************************************************************************
 *                                      Share some buffers                                         *
 **************************************************************************************************/
    _fftGen.setFc(_fc_rel);

    _mainWindow.setData(_fftGen.getFreqVec(), _fftGen.getFftVec());
    _mainWindow.setBuffer(_buffer);
    _mainWindow.setSharedModType(_modType);
    _mainWindow.setFc(_fc_rel);
    _mainWindow.setWindow(_window);

/***************************************************************************************************
 *                                          Start threads.                                         *
 **************************************************************************************************/

    _dataStream->startStream();
    _amcRecv.startDemod(AmcRecv::ReceiveMode::NOTHING);
    _fftGen.startFft();
    _featureExtractor.start(AMC::FeatureExtractor::ExtractionMode::CLASSIFY);

    return _app.exec();
}
