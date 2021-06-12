#pragma once
#include "SceneObject.h"

class Sphere :
    public SceneObject
{
private:
    float radius;
public:
    Sphere(float aRadius, glm::vec3 aPos) 
    {
        radius = aRadius;
        position = aPos;
    }
    CollisionPoint* intersects(const Ray& ray);
};

