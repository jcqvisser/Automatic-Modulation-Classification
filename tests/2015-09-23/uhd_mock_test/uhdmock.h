#ifndef UHDMOCK_H
#define UHDMOCK_H

#include <cmath>
#include "streamer.h"

class UhdMock : public Streamer
{
public:
    explicit UhdMock(double rate, double fc, size_t frameSize = 384);
    void startStream();
    void stopStream();
    boost::shared_ptr < SharedBuffer<std::complex<double> > > getBuffer();
    void setMaxBuffer(size_t maxBuffSize);

private:
    void runStream();

    size_t _maxBuffSize;
    boost::shared_ptr < SharedBuffer<std::complex<double> > > _buffer;
    boost::thread _streamThread;
    bool _isStreaming;
    double _rate;
    double _fc;
    size_t _frameSize;
    double _pi;
};

#endif // UHDMOCK_H
