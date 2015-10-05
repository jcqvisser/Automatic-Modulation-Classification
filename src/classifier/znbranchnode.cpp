#include "znbranchnode.h"

ZnBranchNode::ZnBranchNode(ZnNode lNode, ZnNode rNode, AMC::Feature feature) :
    _lNode(lNode), _rNode(rNode), _threshold(0), _isThresholdSet(false), _feature(feature)
{}

AMC::ModType ZnBranchNode::classify(const std::vector<double> &features)
{
    if (!_isThresholdSet)
        throw std::domain_error("Threshold is not defined.");

    if (features[_feature] >= _threshold)
        return _rNode.classify(features);
    else
        return _lNode.classify(features);
}

void ZnBranchNode::train(const std::vector<std::vector<double> > &features, const std::vector<double> &responses)
{

}

void ZnBranchNode::load(const std::vector<ZnThreshold> &thresholds)
{

}
