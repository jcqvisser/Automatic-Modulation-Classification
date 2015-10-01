#ifndef SHAREDQVECTOR
#define SHAREDQVECTOR

#include <QVector>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>

/**
 * @brief Utility class for a thread safe vector, the class stores a vector object
 * as well as a mutex pointer, locks can be created from the lock here to enure that
 * the vector is thread safe.
 *
 * @author Anthony Farquharson - 563648
 * @author Jacques Visser - 457817
 */

template <class T>
class SharedQVector
{
public:
    /**
     * @brief SharedQVector default constructor, initializes default QVector object.
     */
    SharedQVector();

    /**
     * @brief SharedQVector copy constructor, initializes a QVector object from the given QVector object
     * @param The given QVector object to be copied.
     */
    SharedQVector(QVector<T> & copyVec);

    /**
     * @brief SharedQVector copy constructor for QVector, creates the QVector from the given std::vector.
     * @param The input std::vector that will be copied.
     */
    SharedQVector(std::vector<T> & copyVec);

    /**
     * @brief SharedQVector size constructor, creates the QVector of the size given (unsigned short).
     * @param Size for the QVector to be created.
     */
    SharedQVector(unsigned short N);

    /**
     * @brief SharedQVector size constructor, creates the QVector of the size given (unsigned int).
     * @param Size for the QVector to be created.
     */
    SharedQVector(unsigned int N);

    /**
     * @brief SharedQVector size constructor, creates the QVector of the size given (unsigned long).
     * @param Size for the QVector to be created.
     */
    SharedQVector(unsigned long N);

    /**
     * @brief Function to get the mutex object from this shared QVector.
     * @return Shared pointer to the mutex object.
     */
    boost::shared_ptr < boost::shared_mutex > getMutex();

    /**
     * @brief getData function used to get the QVector that stores the data, so that it can be modified, or read.
     * @return A reference to the stored QVector object.
     */
    QVector < T > & getData();

private:
    QVector < T > _vector;

    boost::shared_ptr < boost::shared_mutex > _vectorMutex;
};

/*
 **********************************************************************************************
 * Placed the implementation in the header file, to avoid templating issues with the compiler *
 **********************************************************************************************
 */


template <class T>
SharedQVector<T>::SharedQVector() :
    _vector(),
    _vectorMutex(new boost::shared_mutex())
{

}

template <class T>
SharedQVector<T>::SharedQVector(unsigned short N) :
    _vector(N),
    _vectorMutex(new boost::shared_mutex())
{

}


template <class T>
SharedQVector<T>::SharedQVector(unsigned int N) :
    _vector(N),
    _vectorMutex(new boost::shared_mutex())
{

}

template <class T>
SharedQVector<T>::SharedQVector(unsigned long N) :
    _vector(N),
    _vectorMutex(new boost::shared_mutex())
{

}

template <class T>
SharedQVector<T>::SharedQVector(QVector<T> & copyQVector) :
    _vector(copyQVector),
    _vectorMutex(new boost::shared_mutex())
{

}

template <class T>
SharedQVector<T>::SharedQVector(std::vector<T> & copyVector) :
    _vector(copyVector.size()),
    _vectorMutex(new boost::shared_mutex())
{
    for(unsigned int i = 0; i < copyVector.size(); ++i)
    {
        _vector[i] = copyVector[i];
    }
}

template <class T>
QVector < T > & SharedQVector<T>::getData()
{
    return _vector;
}

template <class T>
boost::shared_ptr < boost::shared_mutex > SharedQVector<T>::getMutex()
{
    return _vectorMutex;
}


#endif // SHAREDQVECTOR

