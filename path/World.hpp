#ifndef World_hpp
#define World_hpp

#define WORLD_SIZE_X 50
#define WORLD_SIZE_Y 50

class World
{
public:
	World();
	virtual ~World();

	virtual int getHeight(int x, int y) const = 0;

private:

};

#endif
