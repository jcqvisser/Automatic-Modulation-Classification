#include "amczndescisiontree.h"

AmcZnDecisionTree::AmcZnDecisionTree(
        boost::shared_ptr<ZnNode> startNode):
    _startNode(startNode)
{}

AMC::ModType AmcZnDecisionTree::classify(const std::vector<double> &predictData)
{
    return AMC::ModType::AM_DSB_FC;
    //TODO implement classify
}

void AmcZnDecisionTree::train(const std::vector<std::vector<double> > &trainData, const std::vector<double> &responses)
{

}

void AmcZnDecisionTree::writeToFile(std::string fileName)
{

}
