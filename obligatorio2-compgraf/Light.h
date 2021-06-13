#pragma once
#include "Color.h"

class Light
{
public:
	Color getColor();
	float getIntensity();
protected:
	Color color;
	float intensity;
};

