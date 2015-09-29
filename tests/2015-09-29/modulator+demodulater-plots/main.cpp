#include "mainwindow.h"
#include "uhdmock.h"
#include "uhdread.h"
#include "streamer.h"
#include "fftgenerator.h"
#include "sharedvector.h"
#include "efunction.h"
#include "cosfunction.h"
#include "amdsbdemod.h"
#include "amdsbfunction.h"
#include "amcrecv.h"
#include "amcdemodulator.h"
#include <QApplication>
#include <QMetaType>
#include <boost/smart_ptr.hpp>

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
    double freq = 2e0;
    double fc = 150e0;
    size_t N = 64;
    size_t frameSize = 16;

    double gain = 1;
    int supp_carrier = 1;
    double mod_index = 0.5;

    // Create data stream object.
    boost::scoped_ptr < UhdMock > _dataStream(new UhdMock(new AmDsbFunction(new cosFunction(freq), mod_index, (fc/rate), supp_carrier), rate, freq, gain, frameSize));
//    boost::scoped_ptr < Streamer > _dataStream(new UhdMock(new AmDsbFunction(new cosFunction(freq), mod_index, (fc/rate), supp_carrier), rate, freq, gain));
//    boost::scoped_ptr < Streamer > _dataStream(new UhdRead(rate, freq, gain));

    // Get shared buffer
    boost::shared_ptr < SharedBuffer<std::complex<double> > > _buffer(_dataStream->getBuffer());

    // Create fft generator function.
    FFTGenerator _fftGen(_buffer, rate, N);

    // Create demodulator object.
    AmcRecv _amcRecv(_buffer, rate, N);
    _amcRecv.setDemod(new AmDsbDemod(mod_index, (fc/rate), supp_carrier));

    _mainWindow.setData(_fftGen.getFreqVec(), _fftGen.getFftVec());
    _mainWindow.setBuffer(_buffer);
    _mainWindow.setInputData(_dataStream->getTimeVec(), _dataStream->getAmpVec());
    _mainWindow.setOutputData(_amcRecv.getTimeVec(), _amcRecv.getAmpVec());

    _dataStream->startStream();
    _amcRecv.startDemod();
    _fftGen.startFft();
    _mainWindow.startDisplay();

    return a.exec();
}
