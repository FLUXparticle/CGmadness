#include "K2Get.hpp"

K2Get::K2Get()
{
	mIndex = -1;
}

K2Get::~K2Get()
{
  // empty
}

int K2Get::decide(int close, int far)
{
	return close;
}

int K2Get::hit(int index)
{
	mIndex = index;
	return -1;
}

int K2Get::miss(int index)
{
	return -1;
}
