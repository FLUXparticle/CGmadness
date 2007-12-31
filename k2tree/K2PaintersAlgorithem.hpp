#ifndef K2PaintersAlgorithem_hpp
#define K2PaintersAlgorithem_hpp

#include "K2Command.hpp"

#include "vector.hpp"

#include <stack>

class K2PaintersAlgorithem : public K2Command {
public:
  K2PaintersAlgorithem(Vector3 viewer, int indices[]);
  virtual ~K2PaintersAlgorithem();

	int decide(int close, int far);
	int hit(int index, const Range& range);
	int miss(int index);
	
	int count() const;

protected:
	std::stack<int> mStack;
	
private:
	Vector3 mViewer;
	int *mIndices;
	
	int mCount;
	
	int pop();
	
};

inline int K2PaintersAlgorithem::count() const
{
	return mCount;
}

#endif
