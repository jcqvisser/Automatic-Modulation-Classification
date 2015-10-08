#include <QCoreApplication>
#include "classifier/amczndescisiontree.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    AmcZnDecisionTree dt();

    return a.exec();
}
