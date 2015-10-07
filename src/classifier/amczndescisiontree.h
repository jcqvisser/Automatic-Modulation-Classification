#ifndef AMCZDECISIONTREE_H
#define AMCZDECISIONTREE_H

#include "amcclassifier.h"
#include "znnode.h"
#include "znbranchnode.h"
#include <fstream>

class AmcZnDecisionTree : public AmcClassifier<double>
{
private:
    boost::shared_ptr<ZnNode> _startNode;
public:
    AmcZnDecisionTree(boost::shared_ptr<ZnNode> startNode);
    AMC::ModType classify(const std::vector<double> &predictData);
    void train(const std::vector<std::vector<double> > &trainData, const std::vector<double> &responses);
    void train(const std::vector<std::vector<double> > &trainData, const std::vector<AMC::ModType> &responses);
    void load(std::string fileName);
    void unload(std::string fileName);
    AMC::ModType doubletoModType(double d);
    std::vector<AMC::ModType> doubletoModType(std::vector<double> d);
};

#endif
