#include "PositionLight.h"

PositionLight::PositionLight(glm::dvec3 position, Color color, float intensity) : position{position}, Light(color, intensity)
{
}

glm::dvec3 PositionLight::getPosition()
{
    return position;
}
