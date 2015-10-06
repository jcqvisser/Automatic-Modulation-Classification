#ifndef MINMAX
#define MINMAX

template <class T>
class MinMax
{
public:
    MinMax();
    MinMax(T min, T max);
    MinMax(const MinMax<T> & cpy);

    void setMin(T min);
    void setMax(T max);
    T getMin() const;
    T getMax() const;

private:
    T _min;
    T _max;

};

template <class T>
MinMax<T>::MinMax() :
    _min((T)0),
    _max((T)0)
{

}

template <class T>
MinMax<T>::MinMax(T min, T max) :
    _min(min),
    _max(max)
{

}

template <class T>
MinMax<T>::MinMax(const MinMax<T> & cpy) :
    _min(cpy.getMin()),
    _max(cpy.getMax())
{

}

template <class T>
void MinMax<T>::setMin(T min)
{
    _min = min;
}

template <class T>
void MinMax<T>::setMax(T max)
{
    _max = max;
}

template <class T>
T MinMax<T>::getMin() const
{
    return _min;
}

template <class T>
T MinMax<T>::getMax() const
{
    return _max;
}

#endif // MINMAX

