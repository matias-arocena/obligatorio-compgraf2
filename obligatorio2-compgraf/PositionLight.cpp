#include "PositionLight.h"

PositionLight::PositionLight(glm::vec3 position, Color color, float intensity) : position{position}, Light(color, intensity)
{
}

glm::vec3 PositionLight::getPosition()
{
    return position;
}
