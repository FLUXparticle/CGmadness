#ifndef Range_hpp
#define Range_hpp

struct Range
{
	int startX;
	int startY;
	int sizeX;
	int sizeY;
	
	int left;
	int right;

	Range(int startX, int startY, int sizeX, int sizeY)
	{
		this->startX = startX;
		this->startY = startY;
		this->sizeX = sizeX;
		this->sizeY = sizeY;
		
		left = 0;
		right = 0;
	}
};

#endif
