#include "fftgenerator.h"

FFTGenerator::FFTGenerator(boost::shared_ptr < SharedBuffer<std::complex<double> > > buffer, double rate, size_t N):
    _buffer(buffer),
    _fftVec(new SharedQVector<double>(N)),
    _freqVec(new SharedQVector<double>(N)),
    _fftThread(),
    _rate(rate),
    _N(N),
    _isPerformFft(false),
    _fftBuff((fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N)),
    _fftRes((fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N)),
    _plan(fftw_plan_dft_1d(_N,
                           _fftBuff,
                           _fftRes,
                           FFTW_FORWARD,
                           FFTW_ESTIMATE))
{

}

FFTGenerator::~FFTGenerator()
{
    _isPerformFft = false;

    fftw_free(_fftBuff);
    fftw_free(_fftRes);

    fftw_destroy_plan(_plan);
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
    for(unsigned int n = 0; n < _N; ++n)
    {
        _freqVec->getData()[n] = (n - _N / 2) * _rate / (_N - 1);
    }

    while(_isPerformFft)
    {
        if(getTempBuffer())
        {
            fftw_execute(_plan);

            // Get unique access.
            boost::shared_ptr < boost::shared_mutex > mutex = _fftVec->getMutex();
            boost::unique_lock < boost::shared_mutex > lock (*mutex.get());

            for(unsigned int n = 0; n < _N; ++n)
            {
                double absFft = std::sqrt(_fftRes[n][0] * _fftRes[n][0] + _fftRes[n][1] * _fftRes[n][1]);
                if(n < _N/2) {
                    _fftVec->getData()[n + _N/2] = absFft;
                } else {
                    _fftVec->getData()[n - _N/2] = absFft;
                }
            }
        }
    }
}

bool FFTGenerator::getTempBuffer()
{
    // Get unique access.
    boost::shared_ptr < boost::shared_mutex > mutex = _buffer->getMutex();
    boost::shared_lock < boost::shared_mutex > lock (*mutex.get());

    if(_buffer->getBuffer().size() > _N)
    {
        for(unsigned int n = 0; n < _N; ++n)
        {
            _fftBuff[n][0] = _buffer->getBuffer()[n].real();
            _fftBuff[n][1] = _buffer->getBuffer()[n].imag();
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
