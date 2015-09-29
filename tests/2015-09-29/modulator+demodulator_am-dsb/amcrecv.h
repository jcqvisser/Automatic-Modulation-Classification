#ifndef AMCRECV_H
#define AMCRECV_H

#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include "amcdemodulator.h"
#include "sharedbuffer.h"

/**
 * @brief The AmcRecv class performs the actual receival of data from the USRP, it will do the demodulation of the
 * received data. The AmcRecv class will destructively read from the buffer, as it reads the data will be removed and
 * the demodulated data can be used/stored/analysed/etc.
 */

class AmcRecv
{
public:
    /**
     * @brief AmcRecv This constructor will take in the shared buffer from the data stream object, there is no
     * default constructor as a buffer object is required.
     * @param The buffer is required from the streaming object (inherited Streamer), this buffer will be destructively
     * read from.
     * @param N The size of the N-Point fft and thus size of the number of points to read from the buffer.
     */
    explicit AmcRecv(boost::shared_ptr < SharedBuffer < std::complex < double > > > buffer, size_t N = 2048);

    /**
     * @brief This function will launch the thread that the AmcRecv class will run on. Starts the demod process.
     */
    void startDemod();

    /**
     * @brief Stop the demod loop once it is completed or needs to stop. This will end the loop and close the thread.
     */
    void stopDemod();

    /**
     * @brief This function will set the demodulator, if the modulation scheme changes then the demodulater must be changed.
     * @param The demodulator object that will be used in demodulation, inherits from the AmcDemodulator class.
     */
    void setDemod(AmcDemodulator * demodulator);
    std::string getDemodType();

private:
    void runDemod();
    bool getTempFrame(std::vector < std::complex < double > > & tempFrame);

    bool _isReceiving;
    boost::thread _recvThread;
    boost::scoped_ptr < AmcDemodulator > _demodulator;
    boost::shared_ptr < SharedBuffer < std::complex < double > > > _buffer;
    size_t _N;
};

#endif // AMCRECV_H
