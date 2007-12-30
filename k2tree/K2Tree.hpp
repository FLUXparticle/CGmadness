#ifndef K2Tree_hpp
#define K2Tree_hpp

#include "vector.hpp"

#include "Range.hpp"

#include <vector>

class K2Tree {
public:
  K2Tree(Vector3 origin, int x, int y);
  virtual ~K2Tree();
  
  void set(int x, int y, int start, int end);
  void get(int x, int y, int &start, int &end) const;
  
  int paintersAlgorithem(Vector3 viewer, int indices[]) const;
  
  const Range& getRange(int index) const;

private:
	Vector3 mOrigin;
	
	int mX;
	int mY;
	
	std::vector<Range> mRanges;
	
	int newNode(const Range &range);
	
	void find(int x, int y, class K2Command& cmd) const;
	
	friend class K2Set;
	
};

inline const Range& K2Tree::getRange(int index) const
{
	return mRanges[index];
}

#endif
