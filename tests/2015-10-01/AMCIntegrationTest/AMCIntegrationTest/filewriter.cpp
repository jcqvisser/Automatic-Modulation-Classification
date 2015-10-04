#include "filewriter.h"


void FileWriter::newFile(AMC::ModType modType)
{
    _modType = modType;
    _filename = AMC::toString(modType) + "-" + dateTime() + ".csv";
}

std::string FileWriter::dateTime()
{
    boost::posix_time::ptime t(boost::posix_time::second_clock::local_time());
    return boost::posix_time::to_iso_string(t);
}

void FileWriter::writeToFile(std::vector<double> features)
{
    boost::unique_lock<boost::shared_mutex> lock(_featureLists);
    _featureLists.push_back(features);
    lock.unlock();

    if (!_isWriting)
    {
        _isWriting = true;
        _writerThread =	boost::thread(&FileWriter::run, this);
    }
}

void FileWriter::run()
{
    boost::unique_lock<boost::shared_mutex> lock(_featureLists.getMutex());
    featureFile.open (_filename);
    while (_featureLists.getBuffer().size() > 0)
    {
        std::vector<double> tempFeatures(_featureLists.getBuffer().front());
        _featureLists.getBuffer().pop_front();
        lock.unlock();
        for (auto x:tempFeatures)
        {
            file << x;
            file << ",";
        }
        featureFile << "\n";
        lock.lock();
    }
    FeatureFile.close();
    _isWriting = false;
}


