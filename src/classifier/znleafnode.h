#ifndef ZNLEAFNODE_H
#define ZNLEAFNODE_H

#include "znnode.h"

class ZnLeafNode: public ZnNode
{
public:
    ZnLeafNode(AMC::ModType lType, AMC::ModType rType, AMC::Feature feature);
    ~ZnLeafNode(){}

    AMC::ModType classify(const std::vector<double> &features);

    void train(const std::vector<std::vector<double> > &features, const std::vector<double> &responses);

    void loadThresholds();

    void exportThresholds();

    std::vector<AMC::ModType> getTypes();

private:
    AMC::ModType _lType, _rType;
    bool _isThresholdSet;
    double _threshold;
    AMC::Feature _feature;

};

#endif
