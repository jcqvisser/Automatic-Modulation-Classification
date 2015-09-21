#ifndef SHAREDBUFFER_H
#define SHAREDBUFFER_H

#include <deque>
#include <complex>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>

class SharedBuffer : public std::deque < std::complex < double > >
{
public:
    SharedBuffer();
    boost::shared_ptr < boost::shared_mutex > getMutex();

private:
    boost::shared_ptr < boost::shared_mutex > _bufferMutex;
};

#endif // SHAREDBUFFER_H
