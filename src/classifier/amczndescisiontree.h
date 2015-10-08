#ifndef AMCZDECISIONTREE_H
#define AMCZDECISIONTREE_H

#include "amcclassifier.h"
#include "znnode.h"
#include "znbranchnode.h"
#include "znleafnode.h"
#include <fstream>

class AmcZnDecisionTree : public AmcClassifier<double, AMC::ModType>
{
private:
    boost::shared_ptr<ZnNode> _startNode;
public:
    AmcZnDecisionTree(boost::shared_ptr<ZnNode> startNode);
    AMC::ModType classify(const std::vector<double> &predictData);
    bool train(const std::vector<std::vector<double> > &trainData, const std::vector<AMC::ModType> &responses);
    void load(std::string fileName);
    void save(std::string fileName);
};

#endif
