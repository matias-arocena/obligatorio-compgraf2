#pragma once
#include "Light.h"

#include <glm/glm.hpp>

class PositionLight :
    public Light
{
public:
    PositionLight(glm::dvec3 position, Color color, float intensity);
    glm::dvec3 getPosition(); 
private:
    glm::dvec3 position;
};

