#ifndef ZNNODE_H
#define ZNNODE_H

#include "amc.h"

/**
 * The ZnNode base class is designed to form the basis for AmcZnDecisionTree nodes.
 *
 * @author Anthony Farquharson - 563648
 * @author Jacques Visser - 457817
 */
class ZnNode
{
public:

    /**
     * @brief ZnNode Constructor
     * @param f the feature that the node is concerned with
     */
    ZnNode(AMC::Feature f): _feature(f), _threshold(0), _isThresholdSet(false) {}

    /**
     * @brief Pure virtual function, defines a classify function that, depending on the nature of the node, passes the information down the chain of nodes until the leaf node passes a modtype back.
     * @param The vector of features to be used in data classification.
     * @return A modulation type, as defined in the AMC header.
     */
    virtual AMC::ModType classify(const std::vector<double> &features) = 0;

    /**
     * @brief Pure virtual function used to determine and set the decision threshold in this and all lower nodes.
     * @param A vector of features used to train the nodes.
     * @param A vector of desired responses of corresponding to the feature-vector.
     */
    virtual void train(const std::vector<std::vector<double> > &features, const std::vector<AMC::ModType> &responses) = 0;

    /**
     * @brief getTypes finds the types that all the following leaf nodes are concerned with.
     * @return A vector of modulation types.
     */
    virtual std::vector<AMC::ModType> getTypes() = 0;

    virtual void fromString(std::string s) = 0;
    virtual std::string toString() = 0;

    virtual void load(std::vector<std::string> s) = 0;
    virtual std::vector<std::string> save() = 0;

    AMC::Feature _feature;
    double _threshold;
    bool _isThresholdSet;
};

#endif
