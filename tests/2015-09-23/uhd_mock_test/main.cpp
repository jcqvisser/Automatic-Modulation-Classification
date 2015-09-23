#include "mainwindow.h"
#include "uhdmock.h"
#include "streamer.h"
#include "fftgenerator.h"
#include "sharedvector.h"
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
    double fc = 1e5;
    size_t N = 2048;

    boost::scoped_ptr < Streamer > _dataStream(new UhdMock(rate, fc));
    boost::shared_ptr < SharedBuffer<std::complex<double> > > _buffer(_dataStream->getBuffer());
    FFTGenerator _fftGen(_buffer, N, rate);

    w.setData(_fftGen.getFreqVec(), _fftGen.getFftVec());

    _dataStream->startStream();
    _fftGen.startFft();



    return a.exec();
}
