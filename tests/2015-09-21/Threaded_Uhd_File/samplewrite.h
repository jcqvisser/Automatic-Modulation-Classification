#ifndef SAMPLEWRITE_H
#define SAMPLEWRITE_H

#include <string>
#include <fstream>

#include <boost/thread.hpp>
#include <boost/format.hpp>
#include <boost/smart_ptr.hpp>
#include <complex>
#include <deque>

#include "sharedbuffer.h"

class SampleWrite
{
public:
    SampleWrite(boost::shared_ptr< SharedBuffer > buffer, size_t frameSize = 384, std::string fileName = "samples.csv");
    void startWrite();
    void stopWrite();

private:
    void run();
    void getTempBuffer(std::vector<std::complex<double> > & tempBuff);

    std::string _fileName;
    std::ofstream _fileWriter;
    boost::thread _fileThread;
    boost::shared_mutex _bufferMutex;
    boost::shared_ptr< SharedBuffer > _buffer;
    size_t _frameSize;
    bool _isWriting;

};

#endif // SAMPLEWRITE_H
