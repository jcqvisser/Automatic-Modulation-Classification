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

void ZnLeafNode::train(const std::vector<std::vector<double> > &features, const std::vector<AMC::ModType> &responses)
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
            rSum += features[n0][_feature];
            rSquaredSum += std::pow(features[n0][_feature],2);
        }
        if (_lType == responses[n0])
        {
            ++lN;
            lSum += features[n0][_feature];
            lSquaredSum += std::pow(features[n0][_feature],2);
        }
    }

    if (rN == 0)
        throw std::invalid_argument("no data for " + AMC::toString(_rType) + ". Cannot set all thresholds.");
    if (lN == 0)
        throw std::invalid_argument("no data for " + AMC::toString(_lType) + ". Cannot set all thresholds.");

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

std::string ZnLeafNode::toString()
{
    std::string s = AMC::toString(_lType);
    s.append(";");
    s.append(AMC::toString(_rType));
    s.append(";");
    s.append(AMC::toString(_feature));
    s.append(";");
    s.append(std::to_string(_threshold));
    return s;
}

void ZnLeafNode::fromString(std::string s)
{
    size_t sc = s.find_last_of(";")+1;
    std::string st = s.substr(sc, s.length()-sc);
    _threshold = std::stod(st);
}

void ZnLeafNode::load(std::vector<std::string> s)
{
    std::string s0 = toString();
    for (std::string s1: s)
    {
        size_t sc0 = s0.find_last_of(";");
        size_t sc1 = s1.find_last_of(";");
        std::string ss0 = s0.substr(0, sc0);
        std::string ss1 = s1.substr(0, sc1);
        if (ss0.compare(ss1) == 0)
        {
            ZnLeafNode::fromString(s1);
            return;
        }
    }
}

std::vector<std::string> ZnLeafNode::save()
{
    std::vector<std::string> s(1);
    s[0] = toString();
    return s;
}
