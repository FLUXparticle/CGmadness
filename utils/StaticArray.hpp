#ifndef StaticArray_hpp
#define StaticArray_hpp

#include <cassert>

template<class T>
class StaticArray
{
public:
    typedef const T* const_iterator;

    const_iterator begin() const;
    const_iterator end() const;

public:
    StaticArray(unsigned int size);
    StaticArray(unsigned int size, const T& init);
    StaticArray(const StaticArray& other);
    ~StaticArray();

    const T& operator[] (unsigned int i) const;
    T& operator[] (unsigned int i);

    StaticArray& operator= (const StaticArray& other);

    unsigned int size() const;

private:
    unsigned int mSize;
    T* mData;

};

template<class T>
inline typename StaticArray<T>::const_iterator StaticArray<T>::begin() const
{
    return mData;
}

template<class T>
inline typename StaticArray<T>::const_iterator StaticArray<T>::end() const
{
    return mData + mSize;
}

template<class T>
inline unsigned int StaticArray<T>::size() const
{
    return mSize;
}

template<class T>
inline const T& StaticArray<T>::operator[] (unsigned int i) const
{
    assert(i < mSize);
    return mData[i];
}

template<class T>
inline T& StaticArray<T>::operator[] (unsigned int i)
{
    assert(i < mSize);
    return mData[i];
}

#endif
