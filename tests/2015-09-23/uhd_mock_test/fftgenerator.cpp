#include "fftgenerator.h"

FFTGenerator::FFTGenerator(boost::shared_ptr < SharedBuffer<std::complex<double> > > buffer, size_t N, double rate, double frameSize):
    _buffer(buffer),
    _fftVec(new SharedQVector<double>(N)),
    _freqVec(new SharedQVector<double>(N)),
    _fftThread(),
    _rate(rate),
    _N(N),
    _frameSize(frameSize),
    _isPerformFft(false)
{

}

void FFTGenerator::startFft()
{
    _isPerformFft = true;
    _fftThread = boost::thread(&FFTGenerator::runFft, this);
}

void FFTGenerator::stopFft()
{
    _isPerformFft = false;
    _fftThread.join();
}

void FFTGenerator::runFft()
{
    std::vector < std::complex < double > > tempBuffer(_N);
    std::vector < std::complex < double > > fftFrame(_N);

    for(unsigned int n = 0; n < _N; ++n)
    {
        _freqVec->getData()[n] = (n - _N / 2) * _rate / (_N - 1);
    }

    while(_isPerformFft)
    {
        if(getTempBuffer(tempBuffer))
        {
            fftFrame = AMC::fft(tempBuffer);

            // Get unique access.
            boost::shared_ptr < boost::shared_mutex > mutex = _fftVec->getMutex();
            boost::unique_lock < boost::shared_mutex > lock (*mutex.get());

            for(unsigned int n = 0; n < _N; ++n)
            {
                double absFft = std::sqrt(fftFrame[n].real() * fftFrame[n].real() + fftFrame[n].imag() * fftFrame[n].imag());
                if(n < _N/2) {
                    _fftVec->getData()[n + _N/2] = absFft;
                } else {
                    _fftVec->getData()[n - _N/2] = absFft;
                }
            }

            emit signalFftData();
        }
    }
}

bool FFTGenerator::getTempBuffer(std::vector < std::complex < double > > & tempBuff)
{
    // Get unique access.
    boost::shared_ptr < boost::shared_mutex > mutex = _buffer->getMutex();
    boost::unique_lock < boost::shared_mutex > lock (*mutex.get());

    if(_buffer->getBuffer().size() > tempBuff.size())
    {
        for(unsigned int n = 0; n < tempBuff.size(); ++n)
        {
            tempBuff[n] = _buffer->getBuffer().front();
            _buffer->getBuffer().pop_front();
        }
        return true;
    }
    return false;
}

boost::shared_ptr < SharedQVector<double> > FFTGenerator::getFftVec()
{
    return _fftVec;
}

boost::shared_ptr < SharedQVector<double> > FFTGenerator::getFreqVec()
{
    return _freqVec;
}
