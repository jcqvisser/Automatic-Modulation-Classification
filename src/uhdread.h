#ifndef UHDREAD_H
#define UHDREAD_H

#include <uhd.h>
#include <uhd/types/tune_request.hpp>
#include <uhd/utils/thread_priority.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/exception.hpp>

#include <boost/thread.hpp>
#include <boost/format.hpp>
#include <boost/smart_ptr.hpp>
#include "sharedbuffer.h"
#include "streamer.h"

/**
 * @brief The UhdRead class
 * The UHD read class is used for getting data from the UHD and putting it into a buffer,
 * the buffer is of type SharedBuffer, which has mutex for threading protection. The class
 * can return a boost shared pointer to the buffer object, for access from other classes.
 * The UHD read class is threaded, the thread is started with the startStream() function, and stopped
 * with the stopStream() function.
 * This function will mutex lock the buffer when writing information to it.
 *
 * @author Anthony Farquharson - 563648
 * @author Jacques Visser - 457817
 */

class UhdRead : public Streamer
{
public:
    /**
     * @brief The overloaded constructor takes in the variables required for the set up of the USRP.
     * @param rate The sample rate of the USRP.
     * @param freq The center frequency of the USRP
     * @param gain The gain of the USRP.
     * @param frameSize The size of the frame requested.
     * @param args Any additional arguments for the USRP.
     */
    explicit UhdRead(double rate, double freq, double gain, size_t frameSize = 384, std::string args = "");

    /**
     * @brief startStream The start stream function will launch the thread for the run loop.
     */
    void startStream();

    /**
     * @brief The stopStream function will join the thread with the main thread again, closing the USRP
     * stream and stoping the threads main function loop.
     */
    void stopStream();

    /**
     * @brief The getBuffer function will return a boost shared pointer to the buffer object.
     * @return This function returns the pointer to the buffer object.
     */
    boost::shared_ptr< SharedBuffer<std::complex<double> > > getBuffer();

    /**
     * @brief The setThreadPrioritySafe function is static, used for setting USRP thread priority.
     */
    static void setThreadPrioritySafe();

    /**
     * @brief setMaxBuffer will set the max size that the shared buffer can be, if it exceeds this size then the oldest
     * received data point will be deleted.
     * @param The new max buffer size.
     */
    void setMaxBuffer(size_t maxBuffSize);

private:
    /**
     * @brief The init function performs the initialization of the USRP, called by the constructor.
     * @param rate The sample rate of the USRP.
     * @param freq The center frequency of the USRP
     * @param gain The gain of the USRP.
     */
    void init(double rate, double freq, double gain);

    /**
     * @brief The main run loop that will run on a particular thread.
     */
    void run();

    size_t _maxBuffSize;
    uhd::usrp::multi_usrp::sptr _usrp;
    uhd::rx_streamer::sptr _rxStream;
    uhd::rx_metadata_t _rxMetadata;
    size_t _frameSize;
    boost::shared_ptr< SharedBuffer<std::complex<double> > > _buffer;
    boost::thread _uhdThread;
    volatile bool _isReading;
    boost::shared_mutex _bufferMutex;
};

#endif // UHDREAD_H
