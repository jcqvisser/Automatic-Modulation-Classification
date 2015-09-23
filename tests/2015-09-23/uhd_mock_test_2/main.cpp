#include "mainwindow.h"
#include "uhdmock.h"
#include "uhdread.h"
#include "streamer.h"
#include "fftgenerator.h"
#include "sharedvector.h"
#include "efunction.h"
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
    double freq = 150e3;
    double gain = 10;
    size_t N = 2048;

    boost::scoped_ptr < Streamer > _dataStream(new UhdMock(new eFunction(freq), rate, freq, gain));
//    boost::scoped_ptr < Streamer > _dataStream(new UhdRead(rate, freq, gain));
    boost::shared_ptr < SharedBuffer<std::complex<double> > > _buffer(_dataStream->getBuffer());
    FFTGenerator _fftGen(_buffer, N, rate);

    w.setData(_fftGen.getFreqVec(), _fftGen.getFftVec());
    w.setBuffer(_buffer);

    _dataStream->startStream();
    _fftGen.startFft();

    return a.exec();
}
