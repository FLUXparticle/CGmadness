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
	return mData[nid];
}

template<class T>
T& NodeMap<T>::operator[](const NodeID& nid)
{
	if (!mData[nid])
	{
		mData[nid] = new T();
	}

	return *mData[nid];
}
