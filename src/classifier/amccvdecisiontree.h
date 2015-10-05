#ifndef AMCCVDECISIONTREE_H
#define AMCCVDECISIONTREE_H

#include "amcclassifier.h"

template <class T>
class AmcCvDecisionTree : public AmcClassifier<T>
{
public:
    AmcCvDecisionTree();

    AMC::ModType classify(const std::vector<T> &predictData);
    void train(const std::vector<std::vector<T> > &trainData, const std::vector<T> &responses);
};

template <class T>
AmcCvDecisionTree<T>::AmcCvDecisionTree()
{

}

template <class T>
void AmcCvDecisionTree<T>::classify(const std::vector<T> &predictData)
{

}

template <class T>
void AmcCvDecisionTree<T>::train(const std::vector<std::vector<T> > &trainData, const std::vector<T> &responses)
{

}

#endif // AMCCVDECISIONTREE_H
