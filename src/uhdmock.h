#ifndef UHDMOCK_H
#define UHDMOCK_H

#include <boost/thread.hpp>
#include <boost/timer.hpp>
#include <boost/chrono.hpp>
#include "streamer.h"
#include "streamfunction.h"

class UhdMock : public Streamer
{
public:
    explicit UhdMock(StreamFunction * func, double rate, double fc, double gain, size_t frameSize = 384);
    void startStream();
    void stopStream();
    boost::shared_ptr < SharedBuffer<std::complex<double> > > getBuffer();
    void setMaxBuffer(size_t maxBuffSize);
    void changeFunc(StreamFunction * func);

private:
    void runStream();

    size_t _maxBuffSize;
    boost::shared_ptr < SharedBuffer<std::complex<double> > > _buffer;
    boost::scoped_ptr < StreamFunction > _func;
    boost::mutex _funcMutex;
    boost::thread _streamThread;
    bool _isStreaming;
    double _rate;
    double _fc;
    double _gain;
    size_t _frameSize;
};

#endif // UHDMOCK_H
