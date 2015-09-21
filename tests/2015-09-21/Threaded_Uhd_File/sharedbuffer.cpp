#include "sharedbuffer.h"

SharedBuffer::SharedBuffer() :
    std::deque < std::complex < double > >(),
    _bufferMutex(new boost::shared_mutex())
{

}

boost::shared_ptr < boost::shared_mutex > SharedBuffer::getMutex()
{
    return _bufferMutex;
}
