#include "mainwindow.h"
#include <QApplication>
#include <fftw3.h>
#include <qcustomplot.h>
#include <math.h>
#include <amc.h>

#define PI 3.141592654

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    size_t N = 2048;
    double fs = 10e3;
    double fc = 0.02e3;

    std::vector<double> t(N);
    std::vector<double> x(N);
    std::vector<std::complex<double> > xc(N);
    std::vector<std::complex<double> > Xc(N);
    std::vector<std::complex<double> > xIfftC(N);
    std::vector<double> xInstPhase(N);
    std::vector<double> xInstPhaseUnwrapped(N);
    std::vector<double> xNonLinearInstPhaseUnwrapped(N);

    for (size_t n = 0; n < N; ++n)
    {
        t[n] = n/fs;
        x[n] = std::sin(2*PI*fc*t[n]);
        xc[n] = std::complex<double>(x[n], 0);
    }


    Xc = AMC::fft(xc);
    xIfftC = AMC::ifft(Xc);
    xInstPhase = AMC::instantaneousPhase(xc);
    xInstPhaseUnwrapped = AMC::unwrappedInstantaneousPhase(xc);
    xNonLinearInstPhaseUnwrapped = AMC::nonLinearUnwrappedInstantaneousPhase(xc, fc, fs);







    QVector<double> tQ = QVector<double>::fromStdVector(t);
    QVector<double> xQ = QVector<double>::fromStdVector(x);
    QCustomPlot* qcp0 = w.findChild<QCustomPlot*>("qcp0");
    qcp0->addGraph();
    qcp0->graph(0)->setData(tQ, xQ);
    qcp0->xAxis->setRange(0,N/fs);
    qcp0->yAxis->setRange(-1,1);
    qcp0->replot();


    std::vector<double> Xcr(N);
    std::vector<double> f(N);
    for (size_t n = 0; n < N; ++n)
    {
        Xcr[n] = std::abs(Xc[n]);
        f[n] = n*fs/N;
    }
    QVector<double> fQ = QVector<double>::fromStdVector(f);
    QVector<double> XcrQ = QVector<double>::fromStdVector(Xcr);
    QCustomPlot* qcp1 = w.findChild<QCustomPlot*>("qcp1");
    qcp1->addGraph();
    qcp1->graph(0)->setData(fQ,XcrQ);
    qcp1->xAxis->setRange(0,fs);
    qcp1->yAxis->setRange(0,1);
    qcp1->replot();


    std::vector<double> xIfftCR(N);
    for (size_t n = 0; n < N; ++n)
    {
       xIfftCR[n] = std::real(xIfftC[n]);
    }
    QVector<double> xIfftCRQ = QVector<double>::fromStdVector(xIfftCR);
    QCustomPlot* qcp2 = w.findChild<QCustomPlot*>("qcp2");
    qcp2->addGraph();
    qcp2->graph(0)->setData(tQ,xIfftCRQ);
    qcp2->xAxis->setRange(0,N/fs);
    qcp2->yAxis->setRange(-1,1);
    qcp2->replot();


    QVector<double> xInstPhaseQ = QVector<double>::fromStdVector(xInstPhase);
    QCustomPlot* qcp3 = w.findChild<QCustomPlot*>("qcp3");
    qcp3->addGraph();
    qcp3->graph(0)->setData(tQ,xInstPhaseQ);
    qcp3->xAxis->setRange(0,N/fs);
    qcp3->yAxis->setRange(-4,4);
    qcp3->replot();

    QVector<double> xInstPhaseUnwrappedQ = QVector<double>::fromStdVector(xInstPhaseUnwrapped);
    QCustomPlot* qcp4 = w.findChild<QCustomPlot*>("qcp4");
    qcp4->addGraph();
    qcp4->graph(0)->setData(tQ,xInstPhaseUnwrappedQ);
    qcp4->xAxis->setRange(0,N/fs);
    qcp4->yAxis->setRange(-4,80);
    qcp4->replot();

    QVector<double> xNonLinearInstPhaseUnwrappedQ = QVector<double>::fromStdVector(xNonLinearInstPhaseUnwrapped);
    QCustomPlot* qcp5 = w.findChild<QCustomPlot*>("qcp5");
    qcp5->addGraph();
    qcp5->graph(0)->setData(tQ,xNonLinearInstPhaseUnwrappedQ);
    qcp5->xAxis->setRange(0,N/fs);
    qcp5->yAxis->setRange(-1.7,-1.5);
    qcp5->replot();
    return a.exec();
}
