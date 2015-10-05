#include <QCoreApplication>
#include "amc.h"
#include "classifier/amcclassifier.h"
#include "classifier/amczndescisiontree.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    return a.exec();
}
