#include "filewriter.h"

FileWriter::FileWriter() :
    _isWriting(false)
{}

FileWriter::~FileWriter()
{

}

void FileWriter::start()
{
    _isWriting = true;
    _writerThread =	boost::thread(&FileWriter::run, this);
}

void FileWriter::stop()
{
    _isWriting = false;
    _writerThread.join();
}

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

void FileWriter::writeToFile(const std::vector<double> &features)
{
    boost::unique_lock<boost::shared_mutex> lock(*_featureLists.getMutex());
    _featureLists.getBuffer().push_back(features);
    lock.unlock();
}

void FileWriter::run()
{
    boost::unique_lock<boost::shared_mutex> lock(*_featureLists.getMutex());
    file.open(_filename, std::ofstream::app);

    while(_isWriting)
    {
        if(_featureLists.getBuffer().size() > 0)
        {
            std::vector<double> tempFeatures(_featureLists.getBuffer().front());

            _featureLists.getBuffer().pop_front();
            lock.unlock();
            for (auto x:tempFeatures)
            {
                file << x;
                file << ",";
            }
            file << "\n";
        }
        else
        {
            lock.unlock();
            boost::this_thread::sleep_for(boost::chrono::nanoseconds((long)(1e6)));
        }
        lock.lock();
    }

    // Write out the feature list before closing the program.
    while (_featureLists.getBuffer().size() > 0)
    {
        std::vector<double> tempFeatures(_featureLists.getBuffer().front());

        _featureLists.getBuffer().pop_front();
        for (auto x:tempFeatures)
        {
            file << x;
            file << ",";
        }
        file << "\n";
    }
    file.close();
}


