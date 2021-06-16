#include "Light.h"

Light::Light(Color color, float intensity) : color{color}, intensity{intensity}
{
}

Color Light::getColor()
{
    return color;
}

float Light::getIntensity()
{
    return intensity;
}
