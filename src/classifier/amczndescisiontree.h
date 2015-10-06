#ifndef AMCZDECISIONTREE_H
#define AMCZDECISIONTREE_H

#include "amcclassifier.h"
#include "znnode.h"
#include "znbranchnode.h"

class AmcZnDecisionTree : public AmcClassifier<double>
{
private:
    boost::shared_ptr<ZnNode> _startNode;
public:
    AmcZnDecisionTree(boost::shared_ptr<ZnNode> startNode);
    AMC::ModType classify(const std::vector<double> &predictData);
    void train(const std::vector<std::vector<double> > &trainData, const std::vector<double> &responses);
    void writeToFile(std::string fileName);
};

#endif
