#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <iostream>
#include <fstream>
#include <string>
#include "amc.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include "sharedbuffer.h"

class FileWriter
{
public:
    FileWriter();
    ~FileWriter();
    void newFile(AMC::ModType modType);
    void writeToFile(const std::vector<double> &features);
    std::string dateTime();
private:
    AMC::ModType _modType;
    std::string _filename;
    std::ofstream file;

    bool _isWriting = false;
    boost::thread _writerThread;

    SharedBuffer<std::vector<double> > _featureLists;

    void run();

};

#endif // FILEWRITER_H
