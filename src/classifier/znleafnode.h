#ifndef ZNLEAFNODE_H
#define ZNLEAFNODE_H

#include "znnode.h"
#include <stdexcept>

class ZnLeafNode: public ZnNode
{
private:
    AMC::ModType _lType, _rType;
public:
    ZnLeafNode(AMC::ModType lType,
               AMC::ModType rType,
               AMC::Feature feature);
    ~ZnLeafNode(){}

    virtual AMC::ModType classify(const std::vector<double> &features);

    virtual void train(const std::vector<std::vector<double> > &features, const std::vector<AMC::ModType> &responses);

    virtual std::vector<AMC::ModType> getTypes();

    virtual void fromString(std::string s);
    virtual std::string toString();

    virtual void load(std::vector<std::string> s);
    virtual std::vector<std::string> save();
};

#endif
