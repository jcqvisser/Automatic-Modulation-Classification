#include "znbranchnode.h"

ZnBranchNode::ZnBranchNode(boost::shared_ptr<ZnNode> lNode,
                           boost::shared_ptr<ZnNode> rNode,
                           AMC::Feature feature) :
    ZnNode::ZnNode(feature),
    _lNode(lNode),
    _rNode(rNode)
{}

AMC::ModType ZnBranchNode::classify(const std::vector<double> &features)
{
    if (!_isThresholdSet)
        throw std::domain_error("Threshold is not defined.");

    if (features[_feature] >= _threshold)
        return _rNode->classify(features);
    else
        return _lNode->classify(features);
}

void ZnBranchNode::train(const std::vector<std::vector<double> > &features, const std::vector<double> &responses)
{
    if (features.size() != responses.size())
        throw std::length_error("length of features and responses do not match");
    // TODO dont throw a pure exception

    auto rTypes = _rNode->getTypes();
    auto lTypes = _lNode->getTypes();


    double rSum = 0, rSquaredSum = 0;
    double lSum = 0, lSquaredSum = 0;
    int lN = 0, rN = 0;

    for (size_t n0 = 0; n0 < features.size(); ++n0)
    {
        for (size_t n1 = 0; n1 < rTypes.size(); ++n1)
        {
            if (rTypes[n1] == responses[n0])
            {
                ++rN;
                rSum += features[n0][n1];
                rSquaredSum += std::pow(features[n0][n1],2);
            }
        }
        for (size_t n1 = 0; n1 < lTypes.size(); ++n1)
        {
            if(lTypes[n1] == responses[n0])
            {
                ++lN;
                rSum += features[n0][n1];
                rSquaredSum += std::pow(features[n0][n1],2);
            }
        }
    }

    double lMean = lSum/lN;
    double rMean = rSum/rN;

    double lStdDev = std::sqrt(lSquaredSum/lN - std::pow(lSum/lN, 2));
    double rStdDev = std::sqrt(rSquaredSum/rN - std::pow(rSum/rN, 2));

    _threshold = ((lMean*rStdDev) + (rMean*lStdDev))/(lStdDev+rStdDev);
    _isThresholdSet = true;

    _lNode->train(features, responses);
    _rNode->train(features, responses);
}

std::vector<AMC::ModType> ZnBranchNode::getTypes()
{
    std::vector<AMC::ModType> types = _lNode->getTypes();
    std::vector<AMC::ModType> rTypes = _rNode->getTypes();
    types.insert(types.end(), rTypes.begin(), rTypes.end());
    return types;
}
