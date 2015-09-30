#ifndef UHDMOCK_H
#define UHDMOCK_H

#include <boost/thread.hpp>
#include <boost/timer.hpp>
#include <boost/chrono.hpp>
#include "streamer.h"
#include "streamfunction.h"
#include "sharedqvector.h"

class UhdMock : public Streamer
{
public:
    explicit UhdMock(StreamFunction * func, double rate, double fc, double gain, size_t frameSize = 384);
    void startStream();
    void stopStream();
    boost::shared_ptr < SharedBuffer<std::complex<double> > > getBuffer();
    void setMaxBuffer(size_t maxBuffSize);
    void changeFunc(StreamFunction * func);


    // For testing purposes, return the vectors for plots.
    boost::shared_ptr < SharedQVector < double > > getRealTimeVec() {
        return _realTimeVec;
    }
    boost::shared_ptr < SharedQVector < double > > getRealAmpVec() {
        return _realAmpVec;
    }
    boost::shared_ptr < SharedQVector < double > > getImagTimeVec() {
        return _imagTimeVec;
    }
    boost::shared_ptr < SharedQVector < double > > getImagAmpVec() {
        return _imagAmpVec;
    }

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

    boost::shared_ptr < SharedQVector < double > > _realTimeVec;
    boost::shared_ptr < SharedQVector < double > > _realAmpVec;
    boost::shared_ptr < SharedQVector < double > > _imagTimeVec;
    boost::shared_ptr < SharedQVector < double > > _imagAmpVec;
};

#endif // UHDMOCK_H
