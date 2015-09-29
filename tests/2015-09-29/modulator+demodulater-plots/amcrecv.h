#ifndef AMCRECV_H
#define AMCRECV_H

#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include "amcdemodulator.h"
#include "sharedqvector.h"
#include "sharedbuffer.h"

class AmcRecv
{
public:
    explicit AmcRecv(boost::shared_ptr < SharedBuffer < std::complex < double > > > buffer, double rate, size_t N = 2048);
    void startDemod();
    void stopDemod();
    void setDemod(AmcDemodulator * demodulator);
    std::string getDemodType();


    // For testing purposes, return the vectors for plots.
    boost::shared_ptr < SharedQVector < double > > getTimeVec() {
        return _timeVec;
    }
    boost::shared_ptr < SharedQVector < double > > getAmpVec() {
        return _ampVec;
    }
private:
    void runDemod();
    bool getTempFrame(std::vector < std::complex < double > > & tempFrame);

    bool _isReceiving;
    boost::thread _recvThread;
    boost::scoped_ptr < AmcDemodulator > _demodulator;
    boost::shared_ptr < SharedBuffer < std::complex < double > > > _buffer;
    size_t _N;
    double _rate;

    boost::shared_ptr < SharedQVector < double > > _timeVec;
    boost::shared_ptr < SharedQVector < double > > _ampVec;
};

#endif // AMCRECV_H
