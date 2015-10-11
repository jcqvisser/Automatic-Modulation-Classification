#include "uhdread.h"

UhdRead::UhdRead(double rate, double freq, double gain, size_t frameSize, std::string args) :
    Streamer(),
    _maxBuffSize(16384),
    _usrp(uhd::usrp::multi_usrp::make(args)),
    _rxStream(nullptr),
    _rxMetadata(),
    _frameSize(frameSize),
    _buffer(new SharedBuffer<std::complex<double> >() ),
    _fc(new SharedType<double>),
    _window(new SharedType<double>),
    _uhdThread(),
    _isReading(false)
{
    init(rate, freq, gain);
}

void UhdRead::setThreadPrioritySafe()
{
    uhd::set_thread_priority_safe();
}

void UhdRead::setMaxBuffer(size_t maxBuffSize)
{
    _maxBuffSize = maxBuffSize;
}

void UhdRead::init(double rate, double freq, double gain)
{
    // Set the clock source:
    _usrp->set_clock_source("internal");

    // Set the sample rate:
    std::cout << std::endl
              << boost::format("Setting RX Rate: %f Msps...") % (rate/1e6)
              << std::endl;
    _usrp->set_rx_rate(rate);
    std::cout << boost::format("Actual RX Rate: %f Msps...") % (_usrp->get_rx_rate()/1e6)
              << std::endl << std::endl;

    // Set the center frequency:
    std::cout << boost::format("Setting RX Freq: %f MHz...") % (freq/1e6)
              << std::endl;
    uhd::tune_request_t tune_request(freq);
    _usrp->set_rx_freq(tune_request);
    std::cout << boost::format("Actual RX Freq: %f MHz...") % (_usrp->get_rx_freq()/1e6)
              << std::endl << std::endl;

    // Set the gain:
    std::cout << boost::format("Setting RX Gain: %f dB...") % gain
              << std::endl;
    _usrp->set_rx_gain(gain);
    std::cout << boost::format("Actual RX Gain: %f dB...") % _usrp->get_rx_gain()
              << std::endl << std::endl;

    // Configure the stream:
    uhd::stream_args_t stream_args("fc64"); //complex floats
    uhd::rx_streamer::sptr rx_stream = _usrp->get_rx_stream(stream_args);
    uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);
    stream_cmd.stream_now = true;
    rx_stream->issue_stream_cmd(stream_cmd);

}

void UhdRead::startStream()
{
    // Start the data stream
    uhd::stream_args_t stream_args("fc64"); // Complex doubles
    _rxStream = _usrp->get_rx_stream(stream_args);
    uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);
    stream_cmd.stream_now = true;
    _rxStream->issue_stream_cmd(stream_cmd);
    _isReading = true;

    // Allow usrp setup time.
    boost::this_thread::sleep(boost::posix_time::seconds(1.00));

    // Launch thread
    _uhdThread = boost::thread(&UhdRead::run, this);
}

void UhdRead::stopStream()
{
    // Stop data stream
    uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_STOP_CONTINUOUS);
    stream_cmd.stream_now = false;
    _rxStream->issue_stream_cmd(stream_cmd);

    _isReading = false;

    _uhdThread.join();
}

void UhdRead::run()
{
    size_t numAcceptedSamples = 0;
    size_t numRxSamples = 0;
    std::vector< std::complex<double> > tempBuffer(_frameSize);
    while(_isReading)
    {
        numAcceptedSamples = 0;
        numRxSamples = 0;

        while(numAcceptedSamples < _frameSize)
        {
            numRxSamples = _rxStream->recv(&tempBuffer.front(), _frameSize, _rxMetadata);
            numAcceptedSamples += numRxSamples;
        }

        // Get unique access.
        boost::shared_ptr < boost::shared_mutex > mutex = _buffer->getMutex();
        boost::unique_lock < boost::shared_mutex > lock (*mutex.get());
        for(std::complex<double> & tempSample : tempBuffer)
        {
            _buffer->getBuffer().push_back(tempSample);
            tempSample = 0;

            // Stop buffer from getting too large.
            if(_buffer->getBuffer().size() > _maxBuffSize)
            {
                _buffer->getBuffer().pop_front();
            }
        }
    }
    std::cout << std::endl << "UHD Reader thread closing... " << std::endl;
}

void UhdRead::checkFrame()
{
    // Get center frequency
    boost::shared_lock<boost::shared_mutex> fcLock(*_fc->getMutex());
    double newFc = _fc->getData();
    fcLock.unlock();

    // Get window.
    boost::shared_lock<boost::shared_mutex> winLock(*_fc->getMutex());
    double newWindow = _window->getData();
    winLock.unlock();

    if(newFc != _shadowFc || newWindow != _shadowWindow)
    {
        //TODO: Redesign filter.
    }
    _shadowFc = newFc;
    _shadowWindow = newWindow;
}

boost::shared_ptr< SharedBuffer<std::complex<double> > > UhdRead::getBuffer()
{
    return _buffer;
}

boost::shared_ptr< SharedType<double> > UhdRead::getFc()
{
    return _fc;
}

boost::shared_ptr< SharedType<double> > UhdRead::getWindow()
{
    return _window;
}
