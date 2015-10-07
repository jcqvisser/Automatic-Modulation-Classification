#include <QCoreApplication>
#include <iostream>

#include "classifier/amcclassifier.h"
#include "classifier/amccvdecisiontree.h"
#include "classifiertrainer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    AmcClassifier<double, AMC::ModType> * _amcClassifier = new AmcCvDecisionTree();

    std::string dir = "/home/ants/git/Automatic-Modulation-Classification-ELEN4012/train-data/2015-10-07";

    ClassifierTrainer _trainer(_amcClassifier, dir);

    std::cout << "Starting to train classifier." << std::endl << std::endl;
    _trainer.train();
    std::cout << "Finished Training Classifier." << std::endl << std::endl;

    std::string fileName = "cvTreeStructure";
    std::cout << "Saving trained classifier as : " << fileName << std::endl;
    _trainer.save(fileName);

    return a.exec();
}
