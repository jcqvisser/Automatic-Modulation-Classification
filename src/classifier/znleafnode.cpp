#include "znleafnode.h"

ZnLeafNode::ZnLeafNode(AMC::ModType lGroup, AMC::ModType rGroup, AMC::Feature feature) :
    _lGroup(lGroup), _rGroup(rGroup), _isThresholdSet(false), _threshold(0), _feature(feature)
{}

AMC::ModType ZnLeafNode::classify(const std::vector<double> &features)
{
    if (!_isThresholdSet)
        throw std::domain_error("Threshold is not defined");

    if (features(_feature) >= _threshold)
        return _rGroup;
    else
        return _lGroup;
}

void ZnLeafNode::train(const std::vector<std::vector<double> > &features, const std::vector<double> &responses)
{

}

void ZnLeafNode::load(const std::vector<ZnThreshold> &thresholds)
{

}
