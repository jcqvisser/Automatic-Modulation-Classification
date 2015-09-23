#ifndef STREAMER_H
#define STREAMER_H

#include <boost/smart_ptr.hpp>

#include "sharedbuffer.h"

class Streamer
{
public:
    virtual void startStream() = 0;
    virtual void stopStream() = 0;
    virtual boost::shared_ptr < SharedBuffer< std::complex <double > > > getBuffer() = 0;
    virtual void setMaxBuffer(size_t maxBuffSize) = 0;

    virtual ~Streamer() {}

protected:
    Streamer() {}
};

#endif // STREAMER_H
