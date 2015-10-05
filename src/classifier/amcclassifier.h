#ifndef AMCCLASSIFIER
#define AMCCLASSIFIER

#include <vector>

#include "amc.h"

/**
 * The AmcClassifier base class is designed for machine learning based classifiers to inherit from, giving an interface
 * that allows for classification of given information, as well as training data to train the classifier.
 *
 * @author Anthony Farquharson - 563648
 * @author Jacques Visser - 457817
 */

template <class T>
class AmcClassifier
{
public:
    /**
     * @brief Pure virtual function, defines a classify function, taking in a std::vector of predict data.
     * @param The vector of features to be used in the data prediction.
     *
     * TODO: Change the return type of the vector, should rather be an enum defined by the amc giving the prediction.
     */
    virtual AMC::ModType classify(const std::vector<T> &predictData) = 0;

    /**
     * @brief Pure virtual function, designed for the training of the classifier, taking in a vector of vectors,
     * and a vector of responses, every vector in the vector of vectors should contain a set of features, with a correlating
     * response in the response vector.
     * @param The training data, vector of vectors, every vector in the vector of vectors should contain a set of features, with a correlating
     * response in the response vector.
     * @param The vector of responses, correlated to each vector in the train data input.
     */
    virtual void train(const std::vector< std::vector < T > > &trainData, const std::vector < T > &responses) = 0;

    /**
     * @brief Virtual Destructor.
     */
    virtual ~AmcClassifier() {}
};

#endif // AMCCLASSIFIER

