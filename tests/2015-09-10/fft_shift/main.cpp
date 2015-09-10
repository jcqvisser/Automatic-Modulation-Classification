#include "mainwindow.h"
#include <QApplication>
#include <fftw3.h>
#include <cmath>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    double PI = 4 * atan(1);
    int N = 512; // number of samples
    double fs = 10e3; // sampling frequency
    double fc = 1000; // signal frequency

    double t[N];
    fftw_complex * x = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    double f[N];

    // calculation
    for (int n = 0; n < N; ++n)
    {
        t[n] = n/fs;
        x[n][0] = cos(2*PI*fc*t[n]);
        x[n][1] = 0;

        f[n] = (n - N/2) * fs / (N-1);
    }

    fftw_complex *out;
    fftw_plan plan_forward;

    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    plan_forward = fftw_plan_dft_1d(N, x, out, FFTW_FORWARD, FFTW_ESTIMATE);

    fftw_execute(plan_forward);

    w.plot(f, out, N);

    return a.exec();
}
