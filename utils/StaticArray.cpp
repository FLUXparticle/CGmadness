#include "StaticArray.hpp"

#include <assert.h>

template<class T>
StaticArray<T>::StaticArray(unsigned int size) :
    mSize(size)
{
    mData = new T[mSize];
}

template<class T>
StaticArray<T>::StaticArray(unsigned int size, const T& init) :
    mSize(size)
{
    mData = new T[mSize];

    for (unsigned int i = 0; i < mSize; i++)
    {
        mData[i] = init;
    }
}

template<class T>
StaticArray<T>::StaticArray(const StaticArray& other)
{
    mSize = other.mSize;
    mData = new T[mSize];

    operator=(other);
}

template<class T>
StaticArray<T>::~StaticArray()
{
    delete[] mData;
}

template<class T>
StaticArray<T>& StaticArray<T>::operator= (const StaticArray& other)
{
    assert(mSize == other.mSize);

    for (unsigned int i = 0; i < mSize; i++)
    {
        mData[i] = other.mData[i];
    }

    return *this;
}
