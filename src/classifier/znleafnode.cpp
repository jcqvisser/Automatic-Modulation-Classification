#include "znleafnode.h"

ZnLeafNode::ZnLeafNode(AMC::ModType lType,
                       AMC::ModType rType,
                       AMC::Feature feature) :
    ZnNode::ZnNode(feature),
    _lType(lType),
    _rType(rType)
{}

AMC::ModType ZnLeafNode::classify(const std::vector<double> &features)
{
    if (!_isThresholdSet)
        throw std::domain_error("Threshold is not defined");

    if (features[_feature] >= _threshold)
        return _rType;
    else
        return _lType;
}

void ZnLeafNode::train(const std::vector<std::vector<double> > &features, const std::vector<double> &responses)
{
    if (features.size() != responses.size())
        throw std::length_error("length of features and responses do not match");

    double rSum = 0, lSum = 0, rSquaredSum = 0, lSquaredSum = 0;
    int lN = 0, rN = 0;

    for (size_t n0 = 0; n0 < features.size(); ++n0)
    {
        if (_rType == responses[n0])
        {
            ++rN;
            rSum += features[n0][_rType];
            rSquaredSum += std::pow(features[n0][_rType],2);
        }
        if (_lType == responses[n0])
        {
            ++lN;
            lSum += features[n0][_lType];
            lSquaredSum += std::pow(features[n0][_lType],2);
        }
    }

    double lMean = lSum/lN;
    double rMean = rSum/rN;

    double lStdDev = std::sqrt(lSquaredSum/lN - std::pow(lSum/lN, 2));
    double rStdDev = std::sqrt(rSquaredSum/rN - std::pow(rSum/rN, 2));

    _threshold = ((lMean*rStdDev) + (rMean*lStdDev))/(lStdDev+rStdDev);
    _isThresholdSet = true;
}

std::vector<AMC::ModType> ZnLeafNode::getTypes()
{
    std::vector<AMC::ModType> a(2);
    a[0] = _lType;
    a[1] = _rType;
    return a;
}
