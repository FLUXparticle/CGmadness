#ifndef K2Get_hpp
#define K2Get_hpp

#include "K2Command.hpp"

class K2Get : public K2Command {
public:
  K2Get();
  virtual ~K2Get();

	int decide(int close, int far);
	int hit(int index);
	int miss(int index);
	
	int index() const;
	
private:
	int mIndex;

};

inline int K2Get::index() const
{
	return mIndex;
}

#endif
