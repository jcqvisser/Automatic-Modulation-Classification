#include <QCoreApplication>
#include <iostream>

#include "classifier/amcclassifier.h"
#include "classifier/amccvdecisiontree.h"
#include "classifier/amczndescisiontree.h"
#include "classifiertrainer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::string fileName = "znTreeStructure";

//    AmcClassifier<double, AMC::ModType> * _amcClassifier = new AmcCvDecisionTree(
//                INT_MAX,
//                10,
//                0.01f,
//                true,
//                10,
//                10,
//                true,
//                true,
//                NULL);
    AmcClassifier<double, AMC::ModType> * _amcClassifier = new AmcZnDecisionTree();

    std::string dir = "/home/ants/git/Automatic-Modulation-Classification-ELEN4012/train-data/2015-10-12-3";

    ClassifierTrainer _trainer(_amcClassifier, dir);

    std::cout << "Starting to train classifier." << std::endl << std::endl;
    _trainer.train();
    std::cout << "Finished Training Classifier." << std::endl << std::endl;

    std::cout << "Saving trained classifier as : " << fileName << std::endl;
    _trainer.save(fileName);
    std::cout << "Finished..." << std::endl;

    return a.exec();
}
