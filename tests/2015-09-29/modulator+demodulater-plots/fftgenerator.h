#ifndef FFTGENERATER_H
#define FFTGENERATER_H

#include <boost/thread.hpp>
#include <QVector>
#include <complex>
#include <cmath>
#include "amc.h"
#include "sharedbuffer.h"
#include "mainwindow.h"
#include "sharedvector.h"
#include "sharedqvector.h"

class FFTGenerator
{
public:
    FFTGenerator(boost::shared_ptr < SharedBuffer<std::complex<double> > > buffer, double rate, size_t N = 2048);
    void startFft();
    void stopFft();

    boost::shared_ptr < SharedQVector<double> > getFftVec();
    boost::shared_ptr < SharedQVector<double> > getFreqVec();

private:
    void runFft();
    bool getTempBuffer(std::vector < std::complex < double > > & tempBuff);

    boost::shared_ptr < SharedBuffer<std::complex<double> > > _buffer;
    boost::shared_ptr < SharedQVector<double> > _fftVec;
    boost::shared_ptr < SharedQVector<double> > _freqVec;
    boost::thread _fftThread;
    double _rate;
    double _N;
    bool _isPerformFft;

    MainWindow * _main;
};

#endif // FFTGENERATER_H
