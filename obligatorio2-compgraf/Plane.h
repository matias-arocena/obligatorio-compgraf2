#pragma once
#include "SceneObject.h"
class Plane :
    public SceneObject
{
public:
    Plane(glm::dvec3 normal, glm::dvec3 point, glm::dvec3 up, double length, double height);
    CollisionPoint* intersects(const Ray& ray);

private:
    glm::dvec3 normal;
    glm::dvec3 point;
    glm::dvec3 up;
    glm::dvec3 side;
    double length;
    double height;
};

