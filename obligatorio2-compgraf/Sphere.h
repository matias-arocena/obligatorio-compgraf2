#pragma once
#include "SceneObject.h"

class Sphere :
    public SceneObject
{
private:
    double radius;
public:
    Sphere(double aRadius, glm::dvec3 aPos) 
    {
        radius = aRadius;
        position = aPos;
    }
    CollisionPoint* intersects(const Ray& ray);
};

