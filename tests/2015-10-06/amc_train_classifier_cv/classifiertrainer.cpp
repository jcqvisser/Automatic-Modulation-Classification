#include "classifiertrainer.h"

ClassifierTrainer::ClassifierTrainer(AmcClassifier<double> * classifier) :
    _classifier(classifier),
    _currentPath(boost::filesystem::current_path())
{
    boost::filesystem::directory_iterator end_iter;

    std::cout << "Directory: " << _currentPath.generic_string() << std::endl;
    std::cout << "Detected the following Training Files: " << std::endl << std::endl;

    for(boost::filesystem::directory_iterator dir_iter(_currentPath);
        dir_iter != end_iter;
        ++dir_iter)
    {
        std::string fileName;
        AMC::ModType modType;
        if(boost::filesystem::is_regular_file( dir_iter->status() ))
        {
            fileName = dir_iter->path().filename().generic_string();
            modType = findModTypes(fileName);
            if(modType != AMC::ModType::MODTYPE_NR_ITEMS)
            {
                if(AMC::toString(modType).length() >= 8)
                {
                    std::cout << AMC::toString(modType) << " -\t" << fileName << std::endl;
                }
                else
                {
                    std::cout << AMC::toString(modType) << "\t  -\t" << fileName << std::endl;
                }
                _fileStrings.push_back(fileName);
                _modTypes.push_back(modType);
            }
        }
    }
}

void ClassifierTrainer::train()
{

}

void ClassifierTrainer::save(const std::string & fileName)
{

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
