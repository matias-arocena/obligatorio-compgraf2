#pragma once
#include "Color.h"

class Light
{
public:
	Light(Color color, double intensity);
	Color getColor();
	double getIntensity();
protected:
	Color color;
	double intensity;
};

