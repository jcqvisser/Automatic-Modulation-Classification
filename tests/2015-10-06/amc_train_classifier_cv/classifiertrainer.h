#ifndef CLASSIFIERTRAINER_H
#define CLASSIFIERTRAINER_H

#include <fstream>
#include <boost/smart_ptr.hpp>
#include <boost/filesystem.hpp>

#include "amc.h"
#include "classifier/amcclassifier.h"

class ClassifierTrainer
{
public:
    ClassifierTrainer(AmcClassifier<double, AMC::ModType> * classifier);

    void train();
    void save(const std::string & fileName);

private:
    AMC::ModType findModTypes(const std::string & cmpString);

    boost::scoped_ptr < AmcClassifier<double, AMC::ModType> > _classifier;
    boost::filesystem::path _currentPath;
    std::vector < std::string > _fileStrings;
    std::vector < AMC::ModType > _modTypes;

    std::vector < std::vector < double > > _trainData;
    std::vector < AMC::ModType > _responseData;
};

#endif // CLASSIFIERTRAINER_H
