#include "mainwindow.h"
#include <QApplication>
#include <usrpstuff.h>
#include <qcustomplot.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    std::vector<double> x = usrpStuff::usrpSetup();

    int N = 1024;
    double fs = 10e6;
    QVector<double> tQ(N);
    QVector<double> xQ(N);
    for (int n = 0; n < N; ++n)
    {
        tQ[n] = n/fs;
        xQ[n] = x[n];
    }

    QCustomPlot *qcp0 = w.findChild<QCustomPlot*>("qcp0");
    qcp0->addGraph();
    qcp0->graph(0)->addData(tQ, xQ);
    qcp0->xAxis->setRange(0,N/fs);
    qcp0->yAxis->setRange(0,299);
    qcp0->replot();

    boost::this_thread::sleep(boost::posix_time::seconds(2.0));

    return a.exec();
}



