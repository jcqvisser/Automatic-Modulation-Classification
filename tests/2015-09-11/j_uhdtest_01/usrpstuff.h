#ifndef USRPSTUFF_H
#define USRPSTUFF_H


#include <math.h>
#include <fftw3.h>

#include <boost/format.hpp>
#include <boost/thread.hpp>

#include <uhd/types/tune_request.hpp>
#include <uhd/utils/thread_priority.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/exception.hpp>


class usrpStuff
{
public:
    usrpStuff();
    static std::vector<double> fft(std::vector<double> x, int N);
    static std::vector<double> usrpReceive(uhd::usrp::multi_usrp::sptr usrp, int N);
    static std::vector<double> usrpSetup();
};

#endif // USRPSTUFF_H
