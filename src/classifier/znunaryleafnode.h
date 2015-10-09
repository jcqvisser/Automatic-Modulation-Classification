#ifndef ZNUNARYLEAFNODE_H
#define ZNUNARYLEAFNODE_H

#include "znnode.h"

class ZnUnaryLeafNode : public ZnNode
{
public:
    ZnUnaryLeafNode(AMC::ModType m, AMC::Feature f): ZnNode(f), _modType(m){}
    virtual AMC::ModType classify(const std::vector<double> &features);
    virtual void train(const std::vector<std::vector<double> > &features, const std::vector<AMC::ModType> &responses);
    virtual std::vector<AMC::ModType> getTypes();

    virtual void fromString(std::string s);
    virtual std::string toString();

    virtual void load(std::vector<std::string> s);
    virtual std::vector<std::string> save();
private:
    AMC::ModType _modType;
};

#endif
