#ifndef AMCCVDECISIONTREE_H
#define AMCCVDECISIONTREE_H

#include "amcclassifier.h"

class AmcCvDecisionTree : public AmcClassifier<double>
{
public:
    AmcCvDecisionTree();

    AMC::ModType classify(const std::vector<double> &predictData);
    void train(const std::vector<std::vector<double> > &trainData, const std::vector<double> &responses);
};


#endif // AMCCVDECISIONTREE_H
