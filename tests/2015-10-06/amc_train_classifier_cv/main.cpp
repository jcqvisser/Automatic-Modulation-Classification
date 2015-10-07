#include <QCoreApplication>
#include <iostream>

#include "classifier/amcclassifier.h"
#include "classifier/amccvdecisiontree.h"
#include "classifiertrainer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    AmcClassifier<double, AMC::ModType> * _amcClassifier = new AmcCvDecisionTree();

    ClassifierTrainer _trainer(_amcClassifier);

    std::cout << "Starting to train classifier." << std::endl;
    _trainer.train();
    std::cout << "Finished Training Classifier." << std::endl;

    return a.exec();
}

