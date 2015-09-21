#include "mainwindow.h"
#include <QApplication>

#include "testemitter.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    TestEmitter _emitter;

    QObject::connect(&_emitter, SIGNAL(changeData(double, double)),
                         &w, SLOT(slotSetData(double, double)));

    _emitter.startEmitter();

    return a.exec();
}
