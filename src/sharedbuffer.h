#ifndef SHAREDBUFFER_H
#define SHAREDBUFFER_H

#include <deque>
#include <complex>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>

/**
 * @brief The SharedBuffer class
 * The utility class for the storage of complex samples in a buffer,
 * This class implements the double ended queue STL class, storing with
 * it a mutex object for threading protection.
 *
 * @author Anthony Farquharson - 563648
 * @author Jacques Visser - 457817
 */

template <class T>
class SharedBuffer
{
public:

    /**
     * @brief SharedBuffer
     * Default constructor, initializing data entries to 0.
     */
    SharedBuffer();
    SharedBuffer(std::deque<T> & copyVec);
    SharedBuffer(unsigned short N);
    SharedBuffer(unsigned int N);
    SharedBuffer(unsigned long N);

    /**
     * @brief getMutex
     * A function that gets the mutex object for this object.
     * @return
     * This function returns a boost shared pointer to a mutex object.
     */
    boost::shared_ptr < boost::shared_mutex > getMutex();

    std::deque < T > & getBuffer();

private:
    std::deque < T > _buffer;

    /**
     * @brief _bufferMutex
     * Boost shared pointer to mutex object.
     */
    boost::shared_ptr < boost::shared_mutex > _bufferMutex;
};

/*
 **********************************************************************************************
 * Placed the implementation in the header file, to avoid templating issues with the compiler *
 **********************************************************************************************
 */


template <class T>
SharedBuffer<T>::SharedBuffer() :
    _buffer(),
    _bufferMutex(new boost::shared_mutex())
{

}

template <class T>
SharedBuffer<T>::SharedBuffer(unsigned short N) :
    _buffer(N),
    _bufferMutex(new boost::shared_mutex())
{

}


template <class T>
SharedBuffer<T>::SharedBuffer(unsigned int N) :
    _buffer(N),
    _bufferMutex(new boost::shared_mutex())
{

}

template <class T>
SharedBuffer<T>::SharedBuffer(unsigned long N) :
    _buffer(N),
    _bufferMutex(new boost::shared_mutex())
{

}

template <class T>
SharedBuffer<T>::SharedBuffer(std::deque<T> & copyDeque) :
    _buffer(copyDeque),
    _bufferMutex(new boost::shared_mutex())
{

}

template <class T>
std::deque < T > & SharedBuffer<T>::getBuffer()
{
    return _buffer;
}

template <class T>
boost::shared_ptr < boost::shared_mutex > SharedBuffer<T>::getMutex()
{
    return _bufferMutex;
}


#endif // SHAREDBUFFER_H
