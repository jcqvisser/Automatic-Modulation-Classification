#include "amczndescisiontree.h"

AmcZnDecisionTree::AmcZnDecisionTree(
        boost::shared_ptr<ZnNode> startNode):
    _startNode(startNode)
{}


AMC::ModType AmcZnDecisionTree::classify(const std::vector<double> &predictData)
{
    return _startNode->classify(predictData);
}

bool AmcZnDecisionTree::train(const std::vector<std::vector<double> > &trainData, const std::vector<AMC::ModType> &responses)
{
    _startNode->train(trainData, responses);
    return true;
}

void AmcZnDecisionTree::load(std::string fileName)
{
    std::ifstream f(fileName);
    std::vector<std::string> lA;
    std::string line;

    while (std::getline(f,line))
        lA.push_back(line);

    _startNode->load(lA);
    f.close();
}

void AmcZnDecisionTree::save(std::string fileName)
{
    std::ofstream f(fileName);
    std::vector<std::string> l = _startNode->save();

    for (auto line: l)
        f << line << std::endl;

    f.close();
}
