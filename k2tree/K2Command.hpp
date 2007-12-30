#ifndef K2Command_hpp
#define K2Command_hpp

class K2Command {
public:
  K2Command();
  virtual ~K2Command();

	virtual int decide(int close, int far) = 0;
	virtual int hit(int index) = 0;
	virtual int miss(int index) = 0;
	
private:

};

#endif
