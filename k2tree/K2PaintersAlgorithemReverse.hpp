#ifndef K2PaintersAlgorithemReverse_hpp
#define K2PaintersAlgorithemReverse_hpp

#include "K2PaintersAlgorithem.hpp"

class K2PaintersAlgorithemReverse : public K2PaintersAlgorithem {
public:
  K2PaintersAlgorithemReverse(Vector3 viewer, int indices[]);
  virtual ~K2PaintersAlgorithemReverse();

	int decide(int close, int far);

private:

};

#endif
