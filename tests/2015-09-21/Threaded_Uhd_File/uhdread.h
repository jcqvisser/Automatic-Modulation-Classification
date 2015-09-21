#ifndef UHDREAD_H
#define UHDREAD_H

#include <uhd.h>
#include <uhd/types/tune_request.hpp>
#include <uhd/utils/thread_priority.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/transport/udp_simple.hpp>
#include <uhd/exception.hpp>

#include <boost/thread.hpp>
#include <boost/format.hpp>
#include <boost/smart_ptr.hpp>
#include <sharedbuffer.h>

class UhdRead
{
public:
    UhdRead(double rate, double freq, double gain, size_t frameSize = 384, std::string args = "");
    void startStream();
    void stopStream();
    boost::shared_ptr< SharedBuffer > getBuffer();
    static void setThreadPrioritySafe();

private:
    void init(double rate, double freq, double gain);
    void run();

    uhd::usrp::multi_usrp::sptr _usrp;
    uhd::rx_streamer::sptr _rxStream;
    uhd::rx_metadata_t _rxMetadata;
    size_t _frameSize;
    boost::shared_ptr< SharedBuffer > _buffer;
    boost::thread _uhdThread;
    volatile bool _isReading;
    boost::shared_mutex _bufferMutex;
};

#endif // UHDREAD_H
