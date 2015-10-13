#include "uhdmock.h"

UhdMock::UhdMock(StreamFunction * func, double rate, double gain, size_t frameSize) :
    Streamer(),
    _maxBuffSize(16384),
    _buffer(new SharedBuffer<std::complex<double> >()),
    _func(func),
    _funcMutex(),
    _streamThread(),
    _isStreaming(false),
    _rate(rate),
    _gain(gain),
    _frameSize(frameSize),
    _timer(),
    _fc(new SharedType<double>),
    _window(new SharedType<double>),
    _shadowFc(0.0),
    _shadowWindow(0.0),
    _filter(new FirFilter(0.00f, 0.5f, 128))
{

}

void UhdMock::setMaxBuffer(size_t maxBuffSize)
{
    // Change the max buffer size
    _maxBuffSize = maxBuffSize;
}

boost::shared_ptr < SharedType < double > > UhdMock::getFc()
{
    return _fc;
}

boost::shared_ptr < SharedType < double > > UhdMock::getWindow()
{
    return _window;
}

void UhdMock::startStream()
{
    // Initialize run loop conditional and start main loop.
    _isStreaming = true;
    _streamThread = boost::thread(&UhdMock::runStream, this);
}

void UhdMock::changeFunc(StreamFunction * func)
{
    // Change the function used for data generation, aquire a mutex
    // before changing the function.
    boost::unique_lock < boost::mutex > funcLock(_funcMutex);
    _func.reset(func);
}

void UhdMock::stopStream()
{
    // Set the run loop conditional to false (stop the loop) and
    // join the thread.
    _isStreaming = false;
    _streamThread.join();
}

void UhdMock::runStream()
{
    double period = 1/_rate;
    double t = 0.0;

    boost::timer::nanosecond_type _timeSinceLast = _timer.elapsed().wall;
    unsigned long _timeDiff;

    while(_isStreaming)
    {
        // Sleep for the duration relative to the data rate, so that the rate is approximately right.
        _timeDiff = _timer.elapsed().wall - _timeSinceLast;
        boost::this_thread::sleep_for(boost::chrono::nanoseconds((boost::int_least64_t)(period * 1e9 * _frameSize - _timeDiff)));

        // Check that the filter frame has not moved.
        checkFrame();

        // Get unique access.
        boost::shared_ptr < boost::shared_mutex > mutex = _buffer->getMutex();
        boost::unique_lock < boost::shared_mutex > lock (*mutex.get());
        boost::unique_lock < boost::mutex > funcLock(_funcMutex);

        // Generate a frame of data.
        for(unsigned int n = 0; n < _frameSize; ++n)
        {
            _buffer->getBuffer().push_back(_filter->filter(_func->calc(t) * _gain));

            // Prohibit data buffer from getting too large.
            if(_buffer->getBuffer().size() > _maxBuffSize)
            {
                _buffer->getBuffer().pop_front();
            }

            t += period;
        }
    }
    std::cout << std::endl << "Closing UHD mock thread" << std::endl;
}

void UhdMock::checkFrame()
{
    unsigned int _filterLength = 128;

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
        // Redesign filter.
        if(newWindow > 0.0)
        {
            if(newFc - newWindow / 2 < 0)
                _filter->redesign(0.0f, newFc + newWindow / 2, _filterLength);
            else if(newFc + newWindow / 2 > 0.5f)
                _filter->redesign(newFc - newWindow / 2, 0.5f, _filterLength);
            else
                _filter->redesign(newFc - newWindow / 2, newFc + newWindow / 2, _filterLength);
        }
        else
        {
            _filter->redesign(0.0f, 0.5f, _filterLength);
        }
    }
    _shadowFc = newFc;
    _shadowWindow = newWindow;
}

boost::shared_ptr < SharedBuffer<std::complex<double> > > UhdMock::getBuffer()
{
    return _buffer;
}
