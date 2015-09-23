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
 *
 */

class SharedBuffer : public std::deque < std::complex < double > >
{
public:
    /**
     * @brief SharedBuffer
     * Default constructor, initializing data entries to 0.
     */
    SharedBuffer();
    /**
     * @brief getMutex
     * A function that gets the mutex object for this object.
     * @return
     * This function returns a boost shared pointer to a mutex object.
     */
    boost::shared_ptr < boost::shared_mutex > getMutex();

private:
    /**
     * @brief _bufferMutex
     * Boost shared pointer to mutex object.
     */
    boost::shared_ptr < boost::shared_mutex > _bufferMutex;
};

#endif // SHAREDBUFFER_H
