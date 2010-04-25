#ifndef NodeMap_hpp
#define NodeMap_hpp

#include "World.hpp"

#include "NodeID.hpp"

template<class T>
class NodeMap
{
public:
	NodeMap();
	~NodeMap();

	bool exists(const NodeID& nid) const;
	T& operator[](const NodeID& nid);

private:
	T* mData[WORLD_SIZE];

};

#endif
