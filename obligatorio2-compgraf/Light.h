#pragma once
#include "Color.h"

class Light
{
public:
	Light(Color color, float intensity);
	Color getColor();
	float getIntensity();
protected:
	Color color;
	float intensity;
};

