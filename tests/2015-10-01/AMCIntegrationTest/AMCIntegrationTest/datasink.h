#ifndef DATASINK_H
#define DATASINK_H

#include <boost/thread.hpp>
#include <sharedbuffer.h>
#include "boost/random.hpp"
#include "boost/generator_iterator.hpp"



class DataSink
{
public:
    DataSink(boost::shared_ptr<SharedBuffer<std::complex<double> > > buffer,
             size_t N);
    void start();
    void stop();

private:
    boost::shared_ptr < SharedBuffer<std::complex<double> > > _buffer;
    boost::thread _thread;

    double _N;
    bool _isSinking;

    void sink();
};

#endif // DATASINK_H
