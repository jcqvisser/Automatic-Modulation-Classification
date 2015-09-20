#include "mainwindow.h"
#include <QApplication>
#include <amc.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    AMC::FeatureExtractor* fe = AMC::FeatureExtractor::Instance();

    return a.exec();
}
