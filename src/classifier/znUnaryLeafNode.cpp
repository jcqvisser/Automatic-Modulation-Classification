#include "znUnaryLeafNode.h"
#include <iostream>

AMC::ModType ZnUnaryLeafNode::classify(const std::vector<double> &features)
{
   (void) features; //prevent unused variable warning
   return _modType;
}

void ZnUnaryLeafNode::train(const std::vector<std::vector<double> > &features, const std::vector<AMC::ModType> &responses)
{
    //has no threshold to train
    (void) features; //prevent unused variable warning
    (void) responses;
}

std::vector<AMC::ModType> ZnUnaryLeafNode::getTypes()
{
    std::vector<AMC::ModType> m(1);
    m[0] = _modType;
    return m;
}

void ZnUnaryLeafNode::fromString(std::string s)
{
    // has no threshold to load
    (void) s; //prevent unused variable warning
}
std::string ZnUnaryLeafNode::toString()
{
    // has no threshold to write
    return "1;1;1";
}

void ZnUnaryLeafNode::load(std::vector<std::string> s)
{
    // has no threshold to load
    (void) s; //prevent unused variable warning

}

std::vector<std::string> ZnUnaryLeafNode::save()
{
    // has no threshold to write
    std::vector<std::string> s(1);
    s[0] = toString();
    return s;
}
