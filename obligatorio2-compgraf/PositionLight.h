#pragma once
#include "Light.h"

#include <glm/glm.hpp>

class PositionLight :
    public Light
{
public:
    glm::vec3 getPosition(); 
private:
    glm::vec3 position;
};

