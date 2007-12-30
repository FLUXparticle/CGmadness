#ifndef K2Tree_hpp
#define K2Tree_hpp

#include "vector.hpp"

#include <vector>

class K2Tree {
public:
  K2Tree(Vector3 origin, int x, int y);
  virtual ~K2Tree();
  
  void set(int x, int y, int start, int end);
  void get(int x, int y, int &start, int &end) const;

private:
	Vector3 mOrigin;
	
	int mX;
	int mY;
	
	std::vector<struct Range> mRanges;
	
	int newNode(const struct Range &range);
	
	void find(int x, int y, class K2Command& cmd) const;
	
	friend class K2Set;
	
};

#endif
