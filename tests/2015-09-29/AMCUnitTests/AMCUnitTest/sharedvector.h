#ifndef SHAREDVECTOR_H
#define SHAREDVECTOR_H

#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include <QVector>

template <typename T> class SharedVector
{
public:
    SharedVector();
    SharedVector(std::vector<T> & copyVec);
    SharedVector(unsigned short N);
    SharedVector(unsigned int N);
    SharedVector(unsigned long N);
    SharedVector(QVector<T> & copyVec);

    boost::shared_ptr<boost::shared_mutex> getMutex();
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
