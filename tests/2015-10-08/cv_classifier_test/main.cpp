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

#include "modulators/cosfunction.h"
#include "modulators/amfunction.h"
#include "modulators/fmfunction.h"
#include "modulators/digitalfunction.h"
#include "modulators/mpskfunction.h"
#include "modulators/maskfunction.h"
#include "modulators/mqamfunction.h"
#include "modulators/awgnfunction.h"

#include "demodulators/amcdemodulator.h"
#include "demodulators/amdemod.h"
#include "demodulators/fmdemod.h"
#include "demodulators/digitaldemod.h"
#include "demodulators/mpskdemod.h"
#include "demodulators/maskdemod.h"

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
    AmDemod::SideBand sideBand = AmDemod::SideBand::UPPER;
    unsigned int constSize = 2;
    int supp_carrier = 1;
    double mod_index = 1;
    double fm_mod_index = 20e3/rate;
    double snr = 12.0;

    // Frame size and FFT size.
    size_t N = 2048;
    size_t frameSize = 384;

/***************************************************************************************************
 *                                      Create stream object                                       *
 **************************************************************************************************/

    // AM Stream function.
    StreamFunction * _streamFunction = new AmFunction(new cosFunction(freq), mod_index, rel_fc, sideBand, supp_carrier);

    // FM Stream Function
//    StreamFunction * _streamFunction = new FmFunction(new cosFunction(freq), fm_mod_index, rel_fc);

    // Digital Stream Function
//    StreamFunction * _digiBase = new MPskFunction(constSize);
//    StreamFunction * _digiBase = new MQamFunction(constSize);
//    StreamFunction * _digiBase = new MAskFunction(constSize);
//    StreamFunction * _streamFunction = new DigitalFunction(_digiBase, rel_fs, rel_fc);

    // ------------ Create Streamer Object ------------ //
    StreamFunction * _awgnFunction = new AwgnFunction(_streamFunction, snr, rate, 1e6);
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
    AMC::FeatureExtractor _featureExtractor(_buffer, classifier, rate, _fc_rel, N);

    boost::shared_ptr < SharedType<AMC::ModType> > _modType(_featureExtractor.getSharedModType());

/***************************************************************************************************
 *                                      Create Demodulator                                         *
 **************************************************************************************************/

    // Create demodulator object.
    AmcRecv _amcRecv(_buffer, rate, N);
    _amcRecv.setFc(_fc_rel);
    _amcRecv.setModType(_modType);

    // Am Demodulator.
    _amcRecv.setDemod(new AmDemod(mod_index, rel_fc, sideBand, supp_carrier));

    // Fm Demodulator.
//    _amcRecv.setDemod(new FmDemod(mod_index, rel_fc));

    // MPSK Demodulator
//    _amcRecv.setDemod(new DigitalDemod(new MPskDemod(constSize), rel_fc));

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
    _amcRecv.startDemod();
    _fftGen.startFft();
    _featureExtractor.start(AMC::FeatureExtractor::ExtractionMode::CLASSIFY);

    return _app.exec();
}
