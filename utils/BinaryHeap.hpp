#ifndef BinaryHeap_hpp
#define BinaryHeap_hpp

#include "StaticArray.hpp"

typedef unsigned int ValueType;
typedef unsigned int HeapKeyType;

class BinaryHeap
{
public:
    BinaryHeap(int maxSize);
    virtual ~BinaryHeap();

    void insert(HeapKeyType key, ValueType value);
    void update(ValueType value, HeapKeyType newKey);
    void remove(ValueType value);
    bool contains(ValueType value);
    HeapKeyType getKey(ValueType value);
    ValueType peek();
    ValueType removeFirst();
    ValueType removeFirst(HeapKeyType& key);

    int size() const;

private:
    int mSize;
    int mMaxSize;
    StaticArray<HeapKeyType> mKeys;
    StaticArray<ValueType> mValues;
    StaticArray<int> mMap;

    bool lessThan(int i, int j) const;
    void swap(int i, int j);
    void heapify(int index);

    HeapKeyType minKey() const;

};

inline int BinaryHeap::size() const
{
    return mSize;
}

#endif
