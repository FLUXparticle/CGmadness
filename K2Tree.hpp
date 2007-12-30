#ifndef K2Tree_hpp
#define K2Tree_hpp

#include <vector>

class K2Tree {
public:
  K2Tree(int x, int y);
  virtual ~K2Tree();
  
  void set(int x, int y, int start, int end);
  void get(int x, int y, int &start, int &end);

private:
	int mX;
	int mY;
	
	std::vector<int> mTree;
	
	int newNode();
	
	int find(int x, int y);
	
};

#endif
