#ifndef AMCZDECISIONTREE_H
#define AMCZDECISIONTREE_H

#include "amcclassifier.h"
#include "znnode.h"

class AmcZnDecisionTree : public AmcClassifier<double>
{
public:
    AmcZnDecisionTree();
    AMC::ModType classify(const std::vector<double> &predictData);
    void train(const std::vector<std::vector<double> > &trainData, const std::vector<double> &responses);
    void loadThresholds(std::vector<std::vector<double> >);
private:
    ZnNode _startNode;
};

#endif
