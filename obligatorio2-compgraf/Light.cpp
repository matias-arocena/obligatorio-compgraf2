#include "Light.h"

Light::Light(Color color, double intensity) : color{color}, intensity{intensity}
{
}

Color Light::getColor()
{
    return color;
}

double Light::getIntensity()
{
    return intensity;
}
