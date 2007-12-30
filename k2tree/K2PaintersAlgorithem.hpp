#ifndef K2PaintersAlgorithem_hpp
#define K2PaintersAlgorithem_hpp

#include "K2Command.hpp"

#include "K2Tree.hpp"

#include <stack>

class K2PaintersAlgorithem : public K2Command {
public:
  K2PaintersAlgorithem(const K2Tree& tree, Vector3 viewer, int indices[]);
  virtual ~K2PaintersAlgorithem();

	int decide(int close, int far);
	int hit(int index);
	int miss(int index);
	
	int count() const;

private:
	const K2Tree& mTree;
	Vector3 mViewer;
	int *mIndices;
	
	std::stack<int> mStack;
	int mCount;
	
	int pop();
	
};

inline int K2PaintersAlgorithem::count() const
{
	return mCount;
}

#endif
