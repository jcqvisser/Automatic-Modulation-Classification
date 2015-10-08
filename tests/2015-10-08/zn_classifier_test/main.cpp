#include <QCoreApplication>
#include "classifier/amczndescisiontree.h"
#include "classifier/classifiertrainer.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    AmcClassifier<double, AMC::ModType> *dt0 = new AmcZnDecisionTree();
    std::string dir = "/home/jcq/git/Automatic-Modulation-Classification-ELEN4012/train-data/2015-10-07";
    ClassifierTrainer ct(dt0, dir);

    std::cout << "training" << std::endl;
    ct.train();
    std::cout << "saving" << std::endl;
    ct.save("AmcZnDecisionTree0");

    AmcZnDecisionTree dt1;
    dt1.load("AmcZnDecisionTree0");

    dt1.save("AmcZnDecisionTree1");

    return a.exec();
}
