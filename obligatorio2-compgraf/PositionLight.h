#pragma once
#include "Light.h"

#include <glm/glm.hpp>

class PositionLight :
    public Light
{
public:
    PositionLight(glm::vec3 position, Color color, float intensity);
    glm::vec3 getPosition(); 
private:
    glm::vec3 position;
};

