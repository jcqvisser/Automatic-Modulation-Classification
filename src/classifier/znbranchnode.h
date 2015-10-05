#ifndef ZNBRANCHNODE_H
#define ZNBRANCHNODE_H

#include "znnode.h"

class ZnBranchNode: public ZnNode
{
public:
    ZnBranchNode(ZnNode lNode, ZnNode rNode, AMC::Feature feature);
    ~ZnBranchNode(){};

    AMC::ModType classify(const std::vector<double> &features);

    void train(const std::vector<std::vector<double> > &features, const std::vector<double> &responses);

    void load(const std::vector<ZnThreshold> &thresholds);

private:
    ZnNode _lNode, _rNode;
    double _threshold;
    bool _isThresholdSet;
    AMC::Feature _feature;
};

#endif
