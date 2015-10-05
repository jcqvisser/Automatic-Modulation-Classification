#ifndef ZNLEAFNODE_H
#define ZNLEAFNODE_H

#include "znnode.h"

class ZnLeafNode: public ZnNode
{
public:
    ZnLeafNode(AMC::ModType lGroup, AMC::ModType rGroup, AMC::Feature feature);
    ~ZnBranchNode(){};

    AMC::ModType classify(const std::vector<double> &features);

    void train(const std::vector<std::vector<double> > &features, const std::vector<double> &responses);

    void load(const std::vector<ZnThreshold> &thresholds);

private:
    AMC::ModType _lGroup, _rGroup;
    bool _isThresholdSet;
    double _threshold;
    AMC::Feature _feature;

};

#endif ZNLEAFNODE_H
