#pragma once
#include "SceneObject.h"
class Box :
    public SceneObject
{
public:
    Box(glm::dvec3 minVertex, glm::dvec3 maxVertex);
    CollisionPoint* intersects(const Ray& ray);
private:
    glm::dvec3 minVertex;
    glm::dvec3 maxVertex;
};

