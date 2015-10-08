#ifndef ZNBRANCHNODE_H
#define ZNBRANCHNODE_H

#include <stdexcept>
#include "znnode.h"
#include "amc.h"

class ZnBranchNode: public ZnNode
{
private:
    boost::shared_ptr<ZnNode> _lNode, _rNode;

public:
    ZnBranchNode(boost::shared_ptr<ZnNode> lNode,
                 boost::shared_ptr<ZnNode> rNode,
                 AMC::Feature feature);

    virtual AMC::ModType classify(const std::vector<double> &features);

    virtual void train(const std::vector<std::vector<double> > &features, const std::vector<AMC::ModType> &responses);

    virtual std::vector<AMC::ModType> getTypes();

    virtual void fromString(std::string s);
    virtual std::string toString();

    virtual void load(std::vector<std::string> s);
    virtual std::vector<std::string> save();
};

#endif
