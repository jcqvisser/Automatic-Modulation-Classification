#ifndef ZNLEAFNODE_H
#define ZNLEAFNODE_H

#include "znnode.h"

class ZnLeafNode: public ZnNode
{
private:
    AMC::ModType _lType, _rType;
public:
    ZnLeafNode(AMC::ModType lType,
               AMC::ModType rType,
               AMC::Feature feature);
    ~ZnLeafNode(){}

    AMC::ModType classify(const std::vector<double> &features);

    void train(const std::vector<std::vector<double> > &features, const std::vector<double> &responses);

    std::vector<AMC::ModType> getTypes();
};

#endif
