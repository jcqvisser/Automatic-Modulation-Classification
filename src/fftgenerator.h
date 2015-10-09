#ifndef FFTGENERATER_H
#define FFTGENERATER_H

#include <boost/thread.hpp>
#include <QVector>
#include <complex>
#include <cmath>
#include "amc.h"
#include "sharedbuffer.h"
#include "sharedvector.h"
#include "sharedqvector.h"
#include "sharedtype.h"

/**
 * @brief The FFTGenerator class reads off the shared buffer and generates an fft from the information read.
 * The information is placed into two shared QVectors, for display using Qt + QCustomPlot. The shared vectors
 * contain the frequencies and magnitude information from the fft.
 *
 * @author Anthony Farquharson - 563648
 * @author Jacques Visser - 457817
 */

class FFTGenerator
{
public:

    /**
     * @brief FFTGenerator constructor.
     * @param The shared buffer that information will be read from.
     * @param The sample rate of the system, defines the frequency components.
     * @param The fft point size, i.e N-Point fft.
     */
    FFTGenerator(boost::shared_ptr < SharedBuffer<std::complex<double> > > buffer, double rate, size_t N = 2048);

    /**
     * @brief Start the fft generator thread, initializing the looping variable and the boost thread.
     */
    void startFft();

    /**
     * @brief Stop the FFT generator thread, unsetting the run loop conditional and joining the thread.
     */
    void stopFft();

    /**
     * @brief Function to get the shared vector containing the fft magnitudes.
     * @return Shared pointer to shared qvector of fft magnitude.
     */
    boost::shared_ptr < SharedQVector<double> > getFftVec();

    /**
     * @brief Function to get the shared qvector containing the fft frequencies.
     * @return The shared pointer to the shared qvector of fft frequencies.
     */
    boost::shared_ptr < SharedQVector<double> > getFreqVec();

    void setFc(boost::shared_ptr<SharedType < double > > fc);

    ~FFTGenerator();

private:
    void runFft();
    bool getTempBuffer();

    boost::shared_ptr < SharedBuffer<std::complex<double> > > _buffer;
    boost::shared_ptr < SharedQVector<double> > _fftVec;
    boost::shared_ptr < SharedQVector<double> > _freqVec;
    boost::thread _fftThread;
    double _rate;
    double _N;
    bool _isPerformFft;

    // FFTw variables
    fftw_complex * _fftBuff;
    fftw_complex * _fftRes;
    fftw_plan _plan;

    boost::shared_ptr<SharedType < double > > _fc;
};

#endif // FFTGENERATER_H
