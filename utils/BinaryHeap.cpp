#include "BinaryHeap.hpp"

#include <assert.h>

#ifdef assert
#  undef assert
#endif

#define assert(x)

BinaryHeap::BinaryHeap(int maxSize) :
    mSize(0),
    mMaxSize(maxSize),
    mKeys(mMaxSize),
    mValues(mMaxSize),
    mMap(mMaxSize, mMaxSize)
{
    // empty
}

BinaryHeap::~BinaryHeap()
{
    // empty
}

void BinaryHeap::insert(HeapKeyType key, ValueType value)
{
    int index = mSize++;

    mMap[value] = index;
    mKeys[index] = key;
    mValues[index] = value;

    heapify(index);
}

void BinaryHeap::update(ValueType value, HeapKeyType newKey)
{
    int index = mMap[value];

    if (index < mSize)
    {
        mKeys[index] = newKey;
        heapify(index);
    }
    else
    {
        insert(newKey, value);
    }
}

void BinaryHeap::remove(ValueType value)
{
    int index = mMap[value];

    if (index < mSize)
    {
        int last = --mSize;
        swap(last, index);
        heapify(index);
        mMap[value] = mMaxSize;
    }
}

bool BinaryHeap::contains(ValueType value)
{
    return mMap[value] < mSize;
}

HeapKeyType BinaryHeap::getKey(ValueType value)
{
    return mKeys[mMap[value]];
}

ValueType BinaryHeap::peek()
{
    return mValues[0];
}

ValueType BinaryHeap::removeFirst()
{
    ValueType firstValue = mValues[0];

    int last = --mSize;
    swap(last, 0);
    heapify(0);
    mMap[firstValue] = mMaxSize;

    return firstValue;
}

ValueType BinaryHeap::removeFirst(HeapKeyType& key)
{
    key = mKeys[0];

    ValueType firstValue = removeFirst();

    assert(key <= minKey());

    return firstValue;
}

inline int parent(int index)
{
    return (index - 1) / 2;
}

inline int left(int index)
{
    return (index + 1) * 2 - 1;
}

inline int right(int index)
{
    return (index + 1) * 2;
}

bool BinaryHeap::lessThan(int i, int j) const
{
    return mKeys[i] < mKeys[j];
}

void BinaryHeap::swap(int i, int j)
{
    HeapKeyType tKey = mKeys[i];
    ValueType tValue = mValues[i];

    mKeys[i] = mKeys[j];
    mValues[i] = mValues[j];

    mKeys[j] = tKey;
    mValues[j] = tValue;

    mMap[mValues[i]] = i;
    mMap[mValues[j]] = j;
}

void BinaryHeap::heapify(int index)
{
    int t = index;
    int p = parent(t);
    int l = left(t);
    int r = right(t);

    if (index > 0 && lessThan(t, p))
    {
        swap(t, p);
        heapify(p);
    }
    else if (l < mSize)
    {
        if (r >= mSize || lessThan(l, r))
        {
            if (lessThan(l, t))
            {
                swap(t, l);
                heapify(l);
            }
        }
        else
        {
            if (lessThan(r, t))
            {
                swap(t, r);
                heapify(r);
            }
        }
    }
}

HeapKeyType BinaryHeap::minKey() const
{
    HeapKeyType min = mKeys[0];

    for (int i = 1; i < mSize; i++)
    {
        if (mKeys[i] < min)
        {
            assert(false);
        }
    }

    return min;
}

