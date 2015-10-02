#ifndef STREAMER_H
#define STREAMER_H

#include <boost/smart_ptr.hpp>

#include "sharedbuffer.h"

/**
 * @brief The Streamer abstract base class is inherited from for the data streamer objects, they are designed
 * to generate data and place them onto a shared buffer.
 *
 * @author Anthony Farquharson - 563648
 * @author Jacques Visser - 457817
 */

class Streamer
{
public:
    /**
     * @brief The start stream function is used start the thread for the streamer object, intializing the data
     * generation loop (or the loop where data is read from a driver for example).
     */
    virtual void startStream() = 0;

    /**
     * @brief The stopStream function is used to stop the run loop of the streamer, and rejoin the thread.
     */
    virtual void stopStream() = 0;

    /**
     * @brief The get buffer function returns the shared buffer created by this object.
     * @return Shared buffer object, created by the streamer object.
     */
    virtual boost::shared_ptr < SharedBuffer< std::complex <double > > > getBuffer() = 0;

    /**
     * @brief Set the max buffer size, recommended to be quite large.
     * @param The new size for the buffer, it will not be allowed to exceed this side, with the oldest
     * received sample being deleted if it does.
     */
    virtual void setMaxBuffer(size_t maxBuffSize) = 0;

    /**
     * @brief Virtual destructor.
     */
    virtual ~Streamer() {}

protected:
    Streamer() {}
};

#endif // STREAMER_H
