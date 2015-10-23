#include "testfilewriter.h"

TestFileWriter::TestFileWriter() :
    _isWriting(false)
{}

TestFileWriter::~TestFileWriter()
{

}

void TestFileWriter::start()
{
    _isWriting = true;
    _writerThread =	boost::thread(&TestFileWriter::run, this);
}

void TestFileWriter::stop()
{
    _isWriting = false;
    _writerThread.join();
}

void TestFileWriter::newFile(AMC::ModType modType)
{
    _modType = modType;
    _filename = "test_" + AMC::toString(modType) + "-" + dateTime() + ".csv";
}

std::string TestFileWriter::dateTime()
{
    boost::posix_time::ptime t(boost::posix_time::second_clock::local_time());
    return boost::posix_time::to_iso_string(t);
}

void TestFileWriter::writeToFile(const double &snr, const bool &isCorrect)
{
    boost::unique_lock<boost::shared_mutex> lock(*_writeData.getMutex());
    _writeData.getBuffer().push_back({snr, isCorrect});
    lock.unlock();
}

void TestFileWriter::run()
{
    boost::unique_lock<boost::shared_mutex> lock(*_writeData.getMutex());
    file.open(_filename, std::ofstream::app);

    while(_isWriting)
    {
        if(_writeData.getBuffer().size() > 0)
        {
            TestData tempData(_writeData.getBuffer().front());
            _writeData.getBuffer().pop_front();

            lock.unlock();

            file << std::to_string(tempData.snr) << ",";
            file << (tempData.isCorrect ? "1" : "0") << ",";
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
    while (_writeData.getBuffer().size() > 0)
    {
        TestData tempData(_writeData.getBuffer().front());
        _writeData.getBuffer().pop_front();

        lock.unlock();

        file << std::to_string(tempData.snr) << ",";
        file << (tempData.isCorrect ? "1" : "0") << ",";
        file << "\n";

        lock.lock();
    }

    lock.unlock();
    file.close();
}


