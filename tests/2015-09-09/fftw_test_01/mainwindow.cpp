#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <math.h>
#include <fftw3.h>

#define PI 3.14159265

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // setup
    int N = 512; // number of samples
    double fs = 30e3; // sampling frequency
    double fc = 1000; // signal frequency

    double t[N];
    double x[N];

    // calculation
    for (int n = 0; n < N; ++n)
    {
        t[n] = n/fs;
        x[n] = sin(2*PI*fc*t[n]);
    }

    // convert c-style arrays to QVectors
    QVector<double> tQ(N);
    QVector<double> xQ(N);
    qCopy(t, t+N, tQ.begin());
    qCopy(x, x+N, xQ.begin());

    // Plot time-domain signal
    ui->qcp0->addGraph();
    ui->qcp0->graph(0)->setData(tQ,xQ);

    ui->qcp0->xAxis->setRange(0,(N-1)/fs);
    ui->qcp0->xAxis->setLabel("Time (sec)");

    ui->qcp0->yAxis->setRange(-1,1);
    ui->qcp0->yAxis->setLabel("Amplitude");

    ui->qcp0->replot();


    // FFT setup
    fftw_complex *out;
    fftw_plan plan_forward;

    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);

    plan_forward = fftw_plan_dft_r2c_1d(N, x, out, FFTW_ESTIMATE);

    // Execute
    fftw_execute(plan_forward);

    // Complex C-style arrays to magnitude QVector
    QVector<double> XQ(N);
    QVector<double> fQ(N);
    for (int n = 0; n < N; n++)
    {
        XQ[n] =  sqrt((out[n][0] * out[n][0]) + (out[n][1]*out[n][1]))/N;
        fQ[n] = n*fs/N;
    }

    // Plot Frequency-domain signal
    ui->qcp1->addGraph();
    ui->qcp1->graph(0)->setData(fQ,XQ);
    ui->qcp1->xAxis->setRange(-fs,fs);
    ui->qcp1->yAxis->setRange(0,1);
    ui->qcp1->replot();


}

MainWindow::~MainWindow()
{
    delete ui;
}
