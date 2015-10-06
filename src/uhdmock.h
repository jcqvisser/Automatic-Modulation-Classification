#ifndef UHDMOCK_H
#define UHDMOCK_H

#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include "streamer.h"
#include "modulators/streamfunction.h"

/**
 * @brief The UhdMock class is used to stream test data to the system, acting similarily to the UhdRead class.
 * This inherits from the streamer type, so that it is threaded and places samples onto a shared buffer for
 * other classes to access.
 *
 * @author Anthony Farquharson - 563648
 * @author Jacques Visser - 457817
 */

class UhdMock : public Streamer
{
public:
    /**
     * @brief The constructor for UhdMock, initializes the stream information such as rate, frequency and gain.
     * It takes in a function for data generation, of type StreamFunction.
     * @param The function used for data generation, any function inherited from StreamFunction will work.
     * @param The data rate, the UhdMock class will place samples on the buffer at this rate.
     * @param The center frequency of the system, sets the center frequency similar to the UhdRead.
     * @param System gain, any information from the stream function is multiplied by this gain.
     * @param The size of the frames that are generated, data will be generated in chunks of this framesize.
     */
    explicit UhdMock(StreamFunction * func, double rate, double gain, size_t frameSize = 384);

    /**
     * @brief The startStream function will initialize the thread and start the data stream onto the shared buffer object.
     * Starts the run loop of the Streamer.
     */
    void startStream();

    /**
     * @brief The stop stream object will stop the run loop of the program and join the thread back with the main loop.
     */
    void stopStream();

    /**
     * @brief The getBuffer class will return a pointer to the shared buffer created by this class.
     * @return The shared buffer (SharedBuffer object) created by the streamer, so that the information
     * can be used.
     */
    boost::shared_ptr < SharedBuffer<std::complex<double> > > getBuffer();

    /**
     * @brief Set the absolute maximum size of the buffer, is set by default to 16384 samples, if the buffer size exceeds this then the
     * oldest sample is deleted from the buffer automatically.
     * @param The new size for the shared buffer, recommended to be quite large to prevent data loss.
     */
    void setMaxBuffer(size_t maxBuffSize);

    /**
     * @brief Change the function used to generate the data, function is not inherited from Streamer, primarily used for testing purposes
     * as the modulation type may need to be changed.
     * @param The new StreamFunction object for data generation.
     */
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
    double _gain;
    size_t _frameSize;
};

#endif // UHDMOCK_H
