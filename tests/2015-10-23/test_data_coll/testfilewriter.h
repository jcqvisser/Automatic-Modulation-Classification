#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <iostream>
#include <fstream>
#include <string>
#include "amc.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/chrono.hpp>
#include "sharedbuffer.h"
#include "sharedtype.h"

struct TestData
{
    TestData(double cSnr, bool cIsCorrect)
    {
        snr = cSnr;
        isCorrect = cIsCorrect;
    }

    TestData(const TestData & cpyData)
    {
        snr = cpyData.snr;
        isCorrect = cpyData.isCorrect;
    }

    double snr;
    bool isCorrect;
};

class TestFileWriter
{
public:
    TestFileWriter();
    ~TestFileWriter();
    void newFile(AMC::ModType modType);
    void writeToFile(const double & snr, const bool & isCorrect);
    std::string dateTime();

    void start();
    void stop();
private:
    AMC::ModType _modType;
    std::string _filename;
    std::ofstream file;

    bool _isWriting = false;
    boost::thread _writerThread;

    SharedBuffer <TestData> _writeData;

    void run();

};

#endif // FILEWRITER_H
