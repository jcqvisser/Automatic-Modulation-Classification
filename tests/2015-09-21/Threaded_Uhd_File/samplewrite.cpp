#include "samplewrite.h"

SampleWrite::SampleWrite(boost::shared_ptr< SharedBuffer > buffer, size_t frameSize, std::string fileName) :
    _fileName(fileName),
    _fileWriter(),
    _fileThread(),
    _buffer(buffer),
    _frameSize(frameSize),
    _isWriting(false)
{

}

void SampleWrite::startWrite()
{
    _fileWriter.open(_fileName);
    _isWriting = true;

    _fileThread = boost::thread(&SampleWrite::run, this);
}

void SampleWrite::run()
{
    std::vector<std::complex<double> > tempBuff;
    while(_isWriting)
    {
        tempBuff.clear();
        getTempBuffer(tempBuff);

        for(std::complex<double> sample : tempBuff)
        {
            _fileWriter << sample << std::endl;
        }
    }
    std::cout << std::endl << "File writer thread closing... " << std::endl;
}

void SampleWrite::stopWrite()
{
    _isWriting = false;

    _fileThread.join();
}

void SampleWrite::getTempBuffer(std::vector<std::complex<double> > & tempBuff)
{
    // Get unique access.
    boost::shared_ptr < boost::shared_mutex > mutex = _buffer->getMutex();
    boost::unique_lock < boost::shared_mutex > lock (*mutex.get());
    while(!_buffer->empty())
    {
        tempBuff.push_back(_buffer->front());
        _buffer->pop_front();
    }
//    for(std::complex<double> & sample : tempBuff)
//    {
//        if(!_buffer->empty())
//        {
//            sample = _buffer->front();
//            _buffer->pop_front();
//        }
//        else
//        {
//            sample = std::complex<double>(0.0, 0.0);
//        }
//    }
}
