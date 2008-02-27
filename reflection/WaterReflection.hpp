#ifndef WaterReflection_hpp
#define WaterReflection_hpp

class WaterReflection
{
public:
  WaterReflection();
  virtual ~WaterReflection();

	virtual void drawWaterReflection() const = 0;
	
private:

};

#endif
