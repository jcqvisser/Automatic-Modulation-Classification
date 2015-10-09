#ifndef AMCZDECISIONTREE_H
#define AMCZDECISIONTREE_H

#include "amcclassifier.h"
#include "znnode.h"
#include "znbranchnode.h"
#include "znleafnode.h"
#include "znunaryleafnode.h"
#include <fstream>

class AmcZnDecisionTree : public AmcClassifier<double, AMC::ModType>
{
private:
    boost::shared_ptr<ZnNode> _startNode;
public:
    AmcZnDecisionTree();
    AmcZnDecisionTree(boost::shared_ptr<ZnNode> startNode);
    virtual AMC::ModType classify(const std::vector<double> &predictData);
    virtual bool train(const std::vector<std::vector<double> > &trainData, const std::vector<AMC::ModType> &responses);
    virtual void load(const std::string &fileName);
    virtual void save(const std::string &fileName);
};

#endif
