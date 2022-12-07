#ifndef NARRAY_H
#define NARRAY_H

#include <cassert>
#include <vector>
#include <initializer_list>

using std::size_t;

template<typename T>
struct narray
{
private:
    std::vector<T> data;
    size_t narray_size;

public:
    narray();
    narray(const std::initializer_list<T> l);
    narray(const std::vector<T> v);

    void push_back(const T var);
    size_t size() const;
    void clean();

    T& operator[](const size_t i);
    T  operator[](const size_t i) const;

    narray operator+(const narray other) const;
    narray operator-(const narray other) const;
    narray operator*(const T mult) const;
    narray operator/(const T mult) const;

    template<typename T1, typename T2>
    friend narray<T2> operator*(const T1 mult, const narray<T2> arr);
};

template<typename T>
narray<T>::narray(): narray_size(0) {}

template<typename T>
narray<T>::narray(const std::initializer_list<T> l): data(l), narray_size(l.size()) {}

template<typename T>
narray<T>::narray(const std::vector<T> v): data(v), narray_size(v.size()) {}

template<typename T>
void narray<T>::push_back(const T var)
{
    data.push_back(var);
    narray_size++;
    return;
}

template<typename T>
size_t narray<T>::size() const
{
    return narray_size;
}

template<typename T>
void narray<T>::clean()
{
    data.clean();
    narray_size = 0;
    return;
}

template<typename T>
T& narray<T>::operator[](const size_t i)
{
    return this->data[i];
}

template<typename T>
T narray<T>::operator[](const size_t i) const
{
    return this->data[i];
}

template<typename T>
narray<T> narray<T>::operator+(const narray<T> other) const
{
    assert(this->narray_size == other.narray_size);
    narray<T> tmp;
    for (size_t i = 0; i < narray_size; ++i)
        tmp.push_back(this->operator[](i) + other.operator[](i));
    return tmp;
}

template<typename T>
narray<T> narray<T>::operator-(const narray<T> other) const
{
    assert(this->narray_size == other.narray_size);
    narray<T> tmp;
    for (size_t i = 0; i < narray_size; ++i)
        tmp.push_back(this->operator[](i) - other.operator[](i));
    return tmp;
}

template<typename T>
narray<T> narray<T>::operator*(const T mult) const
{
    narray<T> tmp;
    for (size_t i = 0; i < narray_size; ++i)
        tmp.push_back(this->operator[](i) * mult);
    return tmp;
}

template<typename T>
narray<T> narray<T>::operator/(const T mult) const
{
    narray<T> tmp;
    for (size_t i = 0; i < narray_size; ++i)
        tmp.push_back(this->operator[](i) / mult);
    return tmp;
}

template<typename T1, typename T2>
narray<T2> operator*(const T1 mult, const narray<T2> arr)
{
    narray<T2> tmp;
    for (size_t i = 0; i < arr.narray_size; ++i)
        tmp.push_back(arr.operator[](i) * mult);
    return tmp;
}

#endif // NARRAY_H