#include "mainwindow.h"
#include "uhdmock.h"
#include "uhdread.h"
#include "streamer.h"
#include "fftgenerator.h"
#include "sharedvector.h"
#include "efunction.h"
#include "cosfunction.h"
#include "amdemod.h"
#include "amfunction.h"
#include "amcrecv.h"
#include "amcdemodulator.h"
#include <QApplication>
#include <QMetaType>
#include <boost/smart_ptr.hpp>

int main(int argc, char *argv[])
{
    // Signal settings.
    double rate = 1e6;
    double freq = 10e3;
    double fc = 150e3;
    double gain = 1;

    // Modulation settings
    int supp_carrier = 0;
    double mod_index = 0.5;
    AmDemod::SideBand sideBand = AmDemod::SideBand::DOUBLE;

    // Frame size and FFT size.
    size_t N = 2048;
    size_t frameSize = 384;

    // Create data stream object.
    StreamFunction * _streamFunction = new AmFunction(new cosFunction(freq), mod_index, (fc/rate), sideBand, supp_carrier);
    boost::scoped_ptr < Streamer > _dataStream(new UhdMock(_streamFunction, rate, freq, gain, frameSize));
//    boost::scoped_ptr < Streamer > _dataStream(new UhdRead(rate, freq, gain, frameSize));

    // Get shared buffer
    boost::shared_ptr < SharedBuffer<std::complex<double> > > _buffer(_dataStream->getBuffer());

    // Create fft generator function.
    FFTGenerator _fftGen(_buffer, rate, N, frameSize);

    // Create demodulator object.
    AmcRecv _amcRecv(_buffer, N);
    _amcRecv.setDemod(new AmDemod(mod_index, (fc/rate), sideBand, supp_carrier));

    // Initialize interface.
    QApplication a(argc, argv);
    MainWindow _mainWindow;
    _mainWindow.show();

    _mainWindow.setData(_fftGen.getFreqVec(), _fftGen.getFftVec());
    _mainWindow.setBuffer(_buffer);

    _dataStream->startStream();
    _amcRecv.startDemod();
    _fftGen.startFft();

    return a.exec();
}
