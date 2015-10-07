#include "amczndescisiontree.h"

AmcZnDecisionTree::AmcZnDecisionTree(
        boost::shared_ptr<ZnNode> startNode):
    _startNode(startNode)
{}

AMC::ModType AmcZnDecisionTree::doubletoModType(double d)
{
    int m = round(d);
    return (AMC::ModType) m;
}

std::vector<AMC::ModType> AmcZnDecisionTree::doubletoModType(std::vector<double> d)
{
    std::vector<AMC::ModType> m;
    for (double di: d)
    {
        m.push_back(doubletoModType(di));
    }

    return m;
}

AMC::ModType AmcZnDecisionTree::classify(const std::vector<double> &predictData)
{
    return _startNode->classify(predictData);
}

void AmcZnDecisionTree::train(const std::vector<std::vector<double> > &trainData, const std::vector<double> &responses)
{
    auto r = doubletoModType(responses);
    _startNode->train(trainData, r);
}

void AmcZnDecisionTree::train(const std::vector<std::vector<double> > &trainData, const std::vector<AMC::ModType> &responses)
{
    _startNode->train(trainData, responses);
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

void AmcZnDecisionTree::unload(std::string fileName)
{
    std::ofstream f(fileName);
    std::vector<std::string> l = _startNode->unload();

    for (auto line: l)
        f << line << std::endl;

    f.close();
}
