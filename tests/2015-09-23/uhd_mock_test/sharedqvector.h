#ifndef SHAREDQVECTOR
#define SHAREDQVECTOR

#include <QVector>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>

template <class T>
class SharedQVector
{
public:
    SharedQVector();
    SharedQVector(QVector<T> & copyVec);
    SharedQVector(std::vector<T> & copyVec);
    SharedQVector(unsigned short N);
    SharedQVector(unsigned int N);
    SharedQVector(unsigned long N);

    boost::shared_ptr < boost::shared_mutex > getMutex();

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

