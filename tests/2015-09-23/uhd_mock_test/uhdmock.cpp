#include "uhdmock.h"

UhdMock::UhdMock(double rate, double fc, size_t frameSize) :
    Streamer(),
    _maxBuffSize(16384),
    _buffer(new SharedBuffer<std::complex<double> >()),
    _streamThread(),
    _isStreaming(false),
    _rate(rate),
    _fc(fc),
    _frameSize(frameSize),
    _pi(4 * atan(1))
{

}

void UhdMock::setMaxBuffer(size_t maxBuffSize)
{
    _maxBuffSize = maxBuffSize;
}

void UhdMock::startStream()
{
    _isStreaming = true;
    _streamThread = boost::thread(&UhdMock::runStream, this);
}

void UhdMock::stopStream()
{
    _isStreaming = false;
    _streamThread.join();
}

void UhdMock::runStream()
{
    double period = 1/_rate;
    double t = 0.0;
    double real = 0.0;
    double imag = 0.0;
    while(_isStreaming)
    {
        boost::this_thread::sleep(boost::posix_time::seconds(period * _frameSize));

        // Get unique access.
        boost::shared_ptr < boost::shared_mutex > mutex = _buffer->getMutex();
        boost::unique_lock < boost::shared_mutex > lock (*mutex.get());

        // Generate a frame of data.
        for(unsigned int n = 0; n < _frameSize; ++n)
        {
            if(t < 5) {
                real = cos(2 * _pi * _fc * t);
                imag = sin(2 * _pi * _fc * t);
            }
            else if(t < 10 && t >= 5)
            {
                real = cos(2 * _pi * _fc * t);
                imag = 0;
            }
            else
            {
                t = 0.0;
            }

            _buffer->getBuffer().push_back({real, imag});

            // Prohibit data buffer from getting too large.
            if(_buffer->getBuffer().size() > _maxBuffSize)
            {
                _buffer->getBuffer().pop_front();
            }

            t += period;
        }
    }
    std::cout << std::endl << "Closing UHD mock thread" << std::endl;
}

boost::shared_ptr < SharedBuffer<std::complex<double> > > UhdMock::getBuffer()
{
    return _buffer;
}
