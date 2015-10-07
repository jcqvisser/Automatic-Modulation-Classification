#include "classifiertrainer.h"

ClassifierTrainer::ClassifierTrainer(AmcClassifier<double, AMC::ModType> * classifier, std::string dir) :
    _classifier(classifier),
    _currentPath((dir.size() != 0) ? boost::filesystem::path(dir) : boost::filesystem::current_path()),
    _fileStrings(),
    _modTypes(),
    _trainData(),
    _responseData()
{
    if(!boost::filesystem::exists(boost::filesystem::status(_currentPath)))
    {
        std::cout << "Error directory does not exist, defaulting to current path." << std::endl;
        _currentPath = boost::filesystem::current_path();
    }

    if(_currentPath.string().compare(_currentPath.string().size() - 1, 1, "/") != 0)
    {
        _currentPath = boost::filesystem::path(_currentPath.generic_string() + "/");
    }

    std::cout << "Directory: " << _currentPath.generic_string() << std::endl << std::endl;
    std::cout << "Detecting files: " << std::endl << std::endl;

    std::string fileName;
    AMC::ModType modType;

    boost::filesystem::directory_iterator end_iter;
    for(boost::filesystem::directory_iterator dir_iter(_currentPath);
        dir_iter != end_iter;
        ++dir_iter)
    {
        if(boost::filesystem::is_regular_file( dir_iter->status() ))
        {
            fileName = dir_iter->path().filename().generic_string();
            modType = findModTypes(fileName);
            if(modType != AMC::ModType::MODTYPE_NR_ITEMS)
            {
                _fileStrings.push_back(fileName);
                _modTypes.push_back(modType);
            }
        }
    }

    std::string feature_str;
    std::string features_str;
    std::ifstream inptFile;
    std::stringstream featureStream;
    std::vector < double > features_vec;
    for(unsigned int n = 0; (n < _fileStrings.size()) && (n < _modTypes.size()); ++n)
    {
        inptFile.open(_currentPath.generic_string() + _fileStrings[n]);
        std::cout << "Opening File: " << _fileStrings[n] << std::endl;
        if(inptFile.is_open())
        {
            while( std::getline(inptFile, features_str) )
            {
                // Reset string stream.
                featureStream.str(std::string());
                featureStream.clear();
                // Open new string stream.
                featureStream.str(features_str);
                features_vec.clear();
                while( std::getline(featureStream, feature_str, ','))
                {
                    features_vec.push_back(std::stod(feature_str.c_str()));
                }
                _trainData.push_back(features_vec);
                _responseData.push_back(_modTypes[n]);
            }
        }
        inptFile.close();
    }
    std::cout << "Total Feature Groups Found: " << _trainData.size() << std::endl;
    std::cout << std::endl;
}

void ClassifierTrainer::train()
{
    _classifier->train(_trainData, _responseData);
}

void ClassifierTrainer::save(const std::string & fileName)
{
    _classifier->save(fileName);
}

AMC::ModType ClassifierTrainer::findModTypes(const std::string & cmpString)
{
    if(cmpString.find(AMC::toString(AMC::ModType::AM_DSB_FC)) != std::string::npos)
    {
        return AMC::ModType::AM_DSB_FC;
    }
    else if(cmpString.find(AMC::toString(AMC::ModType::AM_DSB_SC)) != std::string::npos)
    {
        return AMC::ModType::AM_DSB_SC;
    }
    else if(cmpString.find(AMC::toString(AMC::ModType::AM_LSB_FC)) != std::string::npos)
    {
        return AMC::ModType::AM_LSB_FC;
    }
    else if(cmpString.find(AMC::toString(AMC::ModType::AM_LSB_SC)) != std::string::npos)
    {
        return AMC::ModType::AM_LSB_SC;
    }
    else if(cmpString.find(AMC::toString(AMC::ModType::AM_USB_FC)) != std::string::npos)
    {
        return AMC::ModType::AM_USB_FC;
    }
    else if(cmpString.find(AMC::toString(AMC::ModType::AM_USB_SC)) != std::string::npos)
    {
        return AMC::ModType::AM_USB_SC;
    }
    else if(cmpString.find(AMC::toString(AMC::ModType::FM)) != std::string::npos)
    {
        return AMC::ModType::FM;
    }
    else if(cmpString.find(AMC::toString(AMC::ModType::PSK_2)) != std::string::npos)
    {
        return AMC::ModType::PSK_2;
    }
    else if(cmpString.find(AMC::toString(AMC::ModType::MPSK)) != std::string::npos)
    {
        return AMC::ModType::MPSK;
    }
    else if(cmpString.find(AMC::toString(AMC::ModType::ASK_2)) != std::string::npos)
    {
        return AMC::ModType::ASK_2;
    }
    else if(cmpString.find(AMC::toString(AMC::ModType::MASK)) != std::string::npos)
    {
        return AMC::ModType::MASK;
    }
    else if(cmpString.find(AMC::toString(AMC::ModType::MQAM)) != std::string::npos)
    {
        return AMC::ModType::MQAM;
    }
    else
    {
        return AMC::ModType::MODTYPE_NR_ITEMS;
    }
}
