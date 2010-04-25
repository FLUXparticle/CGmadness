#include "NodeMap.hpp"

#include "macros.hpp"

template<class T>
NodeMap<T>::NodeMap()
{
	for (unsigned int i = 0; i < LENGTH(mData); i++)
	{
		mData[i] = NULL;
	}
}

template<class T>
NodeMap<T>::~NodeMap()
{
	for (unsigned int i = 0; i < LENGTH(mData); i++)
	{
		delete mData[i];
	}
}

template<class T>
bool NodeMap<T>::exists(const NodeID& nid) const
{
	return mData[nid] != NULL;
}

template<class T>
void NodeMap<T>::put(const NodeID& nid, const T& value)
{
	mData[nid] = new T(value);
}

template<class T>
const T& NodeMap<T>::operator[](const NodeID& nid) const
{
	return *mData[nid];
}
