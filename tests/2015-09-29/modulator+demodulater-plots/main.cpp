#include <QApplication>
#include <QMetaType>
#include <boost/smart_ptr.hpp>

#include "mainwindow.h"
#include "uhdmock.h"
#include "uhdread.h"
#include "streamer.h"
#include "fftgenerator.h"
#include "sharedvector.h"
#include "efunction.h"
#include "cosfunction.h"
#include "amdemod.h"
#include "mpskfunction.h"
#include "mpskdemod.h"
#include "amfunction.h"
#include "amcrecv.h"
#include "amcdemodulator.h"
#include "digitalfunction.h"
#include "fmdemod.h"
#include "fmfunction.h"
#include "digitaldemod.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow _mainWindow;
    _mainWindow.show();

//    double rate = 1e6;
//    double freq = 10e3;
//    double fc = 150e3;
//    size_t N = 2048;
//    size_t frameSize = 384;

    double rate = 1e3;
    double freq = 1e0;
    double fc = 150e0;
    size_t N = 64;
    size_t frameSize = 16;

    double gain = 1;
    int supp_carrier = 1;
    double mod_index = 0.05;
    double rel_fc = fc / rate;
    double rel_fs = freq / rate;
    AmDemod::SideBand sideBand = AmDemod::SideBand::DOUBLE;
    unsigned int constSize = 16;

/***************************************************************************************************
 *                                      Create stream object                                       *
 **************************************************************************************************/

    // AM Stream function.
//    StreamFunction * _streamFunction = new AmFunction(new cosFunction(freq), mod_index, rel_fc, sideBand, supp_carrier);

    // FM Stream Function
//    StreamFunction * _streamFunction = new FmFunction(new cosFunction(freq), mod_index, rel_fc);

    // MPSK Stream Function
    StreamFunction * _streamFunction = new DigitalFunction(new MPskFunction(constSize), rel_fs, rel_fc);

    boost::scoped_ptr < UhdMock > _dataStream(new UhdMock(_streamFunction, rate, freq, gain, frameSize));
//    boost::scoped_ptr < Streamer > _dataStream(new UhdMock(new AmDsbFunction(new cosFunction(freq), mod_index, (fc/rate), supp_carrier), rate, freq, gain));
//    boost::scoped_ptr < Streamer > _dataStream(new UhdRead(rate, freq, gain));

/***************************************************************************************************
 *                                     Initialize utilities                                        *
 **************************************************************************************************/

    // Get shared buffer
    boost::shared_ptr < SharedBuffer<std::complex<double> > > _buffer(_dataStream->getBuffer());

    // Create fft generator function.
    FFTGenerator _fftGen(_buffer, rate, N);

/***************************************************************************************************
 *                                      Create Demodulator                                         *
 **************************************************************************************************/

    // Create demodulator object.
    AmcRecv _amcRecv(_buffer, rate, N);

    // Am Demodulator.
//    _amcRecv.setDemod(new AmDemod(mod_index, rel_fc, sideBand, supp_carrier));

    // Fm Demodulator.
//    _amcRecv.setDemod(new FmDemod(mod_index, rel_fc));

    // MPSK Demodulator
    _amcRecv.setDemod(new DigitalDemod(new MPskDemod(constSize), rel_fs, rel_fc));

 /***************************************************************************************************
  *                                      Initialize GUI Objects                                     *
  **************************************************************************************************/

    _mainWindow.setData(_fftGen.getFreqVec(), _fftGen.getFftVec());
    _mainWindow.setBuffer(_buffer);
    _mainWindow.setInputRealData(_dataStream->getRealTimeVec(), _dataStream->getRealAmpVec());
    _mainWindow.setInputImagData(_dataStream->getImagTimeVec(), _dataStream->getImagAmpVec());
    _mainWindow.setOutputData(_amcRecv.getTimeVec(), _amcRecv.getAmpVec());

/***************************************************************************************************
 *                                          Start threads.                                         *
 **************************************************************************************************/

    _dataStream->startStream();
    _amcRecv.startDemod();
    _fftGen.startFft();
    _mainWindow.startDisplay();

    return a.exec();
}
