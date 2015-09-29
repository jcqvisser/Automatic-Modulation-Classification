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
    qRegisterMetaType<QVector<double>>("QVector<double>&");
    MainWindow w;
    w.show();

    double rate = 1e6;
    double freq = 10e3;
    double gain = 10;
    size_t N = 2048;
    int supp_carrier = 0;
    double mod_index = 0.25;
    double fc = 150e3;

    // Create data stream object.
    boost::scoped_ptr < Streamer > _dataStream(new UhdMock(new AmDsbFunction(new cosFunction(freq), mod_index, (fc/rate), supp_carrier), rate, freq, gain));
//    boost::scoped_ptr < Streamer > _dataStream(new UhdRead(rate, freq, gain));

    // Get shared buffer
    boost::shared_ptr < SharedBuffer<std::complex<double> > > _buffer(_dataStream->getBuffer());

    // Create fft generator function.
    FFTGenerator _fftGen(_buffer, rate, N);

    // Create demodulator object.
    AmcRecv _amcRecv(_buffer);
    _amcRecv.setDemod(new AmDsbDemod(mod_index, fc, supp_carrier));


    w.setData(_fftGen.getFreqVec(), _fftGen.getFftVec());
    w.setBuffer(_buffer);

    _dataStream->startStream();
    _amcRecv.startDemod();
    _fftGen.startFft();

    return a.exec();
}
