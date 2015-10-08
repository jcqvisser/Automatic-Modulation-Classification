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

void ZnBranchNode::train(const std::vector<std::vector<double> > &features, const std::vector<AMC::ModType> &responses)
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
                rSum += features[n0][_feature];
                rSquaredSum += std::pow(features[n0][_feature],2);
            }
        }
        for (size_t n1 = 0; n1 < lTypes.size(); ++n1)
        {
            if(lTypes[n1] == responses[n0])
            {
                ++lN;
                lSum += features[n0][_feature];
                lSquaredSum += std::pow(features[n0][_feature],2);
            }
        }
    }

    if ((lN == 0) || (rN == 0))
        throw std::invalid_argument("Data for at least one modulation scheme is missing, cannot find thresholds.");

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

void ZnBranchNode::load(std::vector<std::string> s)
{
    std::string s0 = toString();
    for (std::string s1: s)
    {
        size_t sc0 = s0.find_last_of(";");
        size_t sc1 = s1.find_last_of(";");
        if (s0.substr(sc0, s0.length()-sc0).compare(s1.substr(sc1, s1.length()-sc1)))
        {
            ZnBranchNode::fromString(s1);
            _lNode->load(s);
            _rNode->load(s);
            return;
        }
    }
}

std::vector<std::string> ZnBranchNode::save()
{
    std::vector<std::string> s;
    s.push_back(ZnBranchNode::toString());
    std::vector<std::string> sl = _lNode->save();
    std::vector<std::string> sr = _rNode->save();

    s.insert(s.end(), sr.begin(), sr.end());
    s.insert(s.end(), sl.begin(), sl.end());

    return s;
}

void ZnBranchNode::fromString(std::string s)
{
    size_t sc = s.find_last_of(";")+1;
    _threshold = stod(s.substr(sc, s.length()-sc));
}

std::string ZnBranchNode::toString()
{
    std::vector<AMC::ModType> lTypes = _lNode->getTypes();
    std::vector<AMC::ModType> rTypes = _rNode->getTypes();

    std::string s;

    for (size_t n = 0; n < lTypes.size(); ++n)
    {
        s.append(AMC::toString(lTypes[n]));
        if (n < lTypes.size()-1)
            s.append(",");
        else
            s.append(";");
    }

    for (size_t n = 0; n < rTypes.size(); ++n)
    {
        s.append(AMC::toString(rTypes[n]));
        if (n < rTypes.size()-1)
            s.append(",");
        else
            s.append(";");
    }

    s.append(AMC::toString(_feature));
    s.append(";");
    s.append(std::to_string(_threshold));

    return s;
}
