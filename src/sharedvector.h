#ifndef SHAREDVECTOR_H
#define SHAREDVECTOR_H

#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include <QVector>

/**
 * @brief Utility class for a thread safe vector, the class stores a vector object
 * as well as a mutex pointer, locks can be created from the lock here to enure that
 * the vector is thread safe.
 *
 * @author Anthony Farquharson - 563648
 * @author Jacques Visser - 457817
 */

template <typename T> class SharedVector
{
public:
    /**
     * @brief SharedVector default constructor, initializes default vector object.
     */
    SharedVector();

    /**
     * @brief SharedVector copy constructor, initializes a vector object from the given vector object
     * @param The given vector object to be copied.
     */
    SharedVector(std::vector<T> & copyVec);

    /**
     * @brief SharedVector size constructor, creates the vector of the size given (unsigned short).
     * @param Size for the vector to be created.
     */
    SharedVector(unsigned short N);

    /**
     * @brief SharedVector size constructor, creates the vector of the size given (unsigned int).
     * @param Size for the vector to be created.
     */
    SharedVector(unsigned int N);

    /**
     * @brief SharedVector size constructor, creates the vector of the size given (unsigned long).
     * @param Size for the vector to be created.
     */
    SharedVector(unsigned long N);

    /**
     * @brief SharedVector copy constructor for QVector, creates the vector from the given QVector.
     * @param The input QVector that will be copied.
     */
    SharedVector(QVector<T> & copyVec);

    /**
     * @brief Function to get the mutex object from this shared vector.
     * @return Shared pointer to the mutex object.
     */
    boost::shared_ptr<boost::shared_mutex> getMutex();

    /**
     * @brief getData function used to get the vector that stores the data, so that it can be modified, or read.
     * @return A reference to the stored vector object.
     */
    std::vector<T> & getData();

private:
    std::vector<T> _sharedVec;
    boost::shared_ptr<boost::shared_mutex> _vecMutex;
};

/*
 **********************************************************************************************
 * Placed the implementation in the header file, to avoid templating issues with the compiler *
 **********************************************************************************************
 */


template <class T>
SharedVector<T>::SharedVector() :
    _sharedVec(),
    _vecMutex(new boost::shared_mutex())
{

}

template <class T>
SharedVector<T>::SharedVector(unsigned short N) :
    _sharedVec(N),
    _vecMutex(new boost::shared_mutex())
{

}

template <class T>
SharedVector<T>::SharedVector(unsigned int N) :
    _sharedVec(N),
    _vecMutex(new boost::shared_mutex())
{

}

template <class T>
SharedVector<T>::SharedVector(unsigned long N) :
    _sharedVec(N),
    _vecMutex(new boost::shared_mutex())
{

}

template <class T>
SharedVector<T>::SharedVector(std::vector<T> & copyVec) :
    _sharedVec(copyVec),
    _vecMutex(new boost::shared_mutex())
{

}

template <class T>
SharedVector<T>::SharedVector(QVector<T> & copyVec) :
    _sharedVec(copyVec.size()),
    _vecMutex(new boost::shared_mutex())
{
    for(unsigned int n = 0; n < copyVec.size(); ++n)
    {
        _sharedVec[n] = copyVec[n];
    }
}

template <class T>
boost::shared_ptr<boost::shared_mutex> SharedVector<T>::getMutex()
{
    return _vecMutex;
}

template <class T>
std::vector<T> & SharedVector<T>::getData()
{
    return _sharedVec;
}

#endif // SHAREDVECTOR_H
