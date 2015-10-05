#ifndef ZNNODE_H
#define ZNNODE_H

#include "amc.h"
#include "znthreshold.h"
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
     * @brief Pure virtual function, defines a classify function that, depending on the nature of the node, passes the information down the chain of nodes until the leaf node passes a modtype back.
     * @param The vector of features to be used in data classification.
     * @return A modulation type, as defined in the AMC header.
     */
    virtual AMC::modtype classify(const std::vector<double> &features);

    /**
     * @brief Pure virtual function used to determine and set the decision threshold in this and all lower nodes.
     * @param A vector of features used to train the nodes.
     * @param A vector of desired responses of corresponding to the feature-vector.
     */
    virtual void train(const std::vector<double> &features, const std::vector<double> &responses);

    /**
     * @brief loadThresholds loads pre-defined thresholds into the known nodes
     * @param thresholds a vector of znThreshold objects.
     */
    virtual void loadThresholds(const std::vector<ZnThreshold> &thresholds);

    /**
     * @brief ~ZnNode, the virtual destructor.
     */
    virtual ~ZnNode() {}

};

#endif
