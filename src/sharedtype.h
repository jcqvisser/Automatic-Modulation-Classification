#ifndef SHAREDTYPE
#define SHAREDTYPE

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

template <class T>
class SharedType
{
public:
    SharedType();
    SharedType(const T & inpt);

    T & getData();
    boost::shared_ptr<boost::shared_mutex> getMutex();
private:
    T _data;
    boost::shared_ptr<boost::shared_mutex> _mutex;

};

template <class T>
SharedType<T>::SharedType():
    _mutex(new boost::shared_mutex)
{

}

template <class T>
SharedType<T>::SharedType(const T & inpt):
    _data(inpt),
    _mutex(new boost::shared_mutex)
{

}

template <class T>
T & SharedType<T>::getData()
{
    return _data;
}

template <class T>
boost::shared_ptr<boost::shared_mutex> SharedType<T>::getMutex()
{
    return _mutex;
}

#endif // SHAREDTYPE

