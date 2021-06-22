#pragma once
#include "SceneObject.h"
class Plane :
    public SceneObject
{
public:
    Plane(glm::dvec3 normal, glm::dvec3 point);
    CollisionPoint* intersects(const Ray& ray);

private:
    glm::dvec3 normal;
    glm::dvec3 point;
};

