#ifndef ZNBRANCHNODE_H
#define ZNBRANCHNODE_H

#include <stdexcept>
#include "znnode.h"

class ZnBranchNode: public ZnNode
{
public:
    ZnBranchNode(boost::shared_ptr<ZnNode> lNode, boost::shared_ptr<ZnNode> rNode, AMC::Feature feature);

    virtual AMC::ModType classify(const std::vector<double> &features);

    virtual void train(const std::vector<std::vector<double> > &features, const std::vector<double> &responses);

    virtual void loadThresholds();

    virtual void exportThresholds();

    virtual std::vector<AMC::ModType> getTypes();

private:
    boost::shared_ptr<ZnNode> _lNode, _rNode;
    double _threshold;
    bool _isThresholdSet;
    AMC::Feature _feature;
};

#endif
