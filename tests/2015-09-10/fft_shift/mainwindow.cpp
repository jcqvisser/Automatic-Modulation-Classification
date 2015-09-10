#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <uhd.h>
#include <cmath>
#include <limits>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->customPlot->addGraph();
}

void MainWindow::plot(double * X, fftw_complex * Y, int N)
{
    QVector<double> qX(N), qY(N);

    double xMin = std::numeric_limits<double>::max();
    double yMin = std::numeric_limits<double>::max();
    double xMax = std::numeric_limits<double>::min();
    double yMax = std::numeric_limits<double>::min();

    for(int n = 0; n < N; ++n) {
        qX[n] = X[n];
        if(n < N/2) {
            qY[n + N/2] = std::sqrt(Y[n][0] * Y[n][0] + Y[n][1] * Y[n][1])/N;
        } else {
            qY[n - N/2] = std::sqrt(Y[n][0] * Y[n][0] + Y[n][1] * Y[n][1])/N;
        }


        xMin = qMin(xMin, qX[n]);
        xMax = qMax(xMax, qX[n]);
        yMin = qMin(yMin, qY[n]);
        yMax = qMax(yMax, qY[n]);
    }

    ui->customPlot->graph(0)->setData(qX, qY);
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(xMin, xMax);
    ui->customPlot->yAxis->setRange(yMin, yMax);

    ui->customPlot->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}
