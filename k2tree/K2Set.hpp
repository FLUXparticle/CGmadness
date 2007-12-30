#ifndef K2Set_hpp
#define K2Set_hpp

#include "K2Command.hpp"

#include "K2Tree.hpp"

class K2Set : public K2Command {
public:
  K2Set(K2Tree& tree);
  virtual ~K2Set();
  
	int decide(int close, int far);
	int hit(int index);
	int miss(int index);
	
	int index() const;
	
private:
	K2Tree& mTree;
	
	int mIndex;
	
};

inline int K2Set::index() const
{
	return mIndex;
}

#endif
