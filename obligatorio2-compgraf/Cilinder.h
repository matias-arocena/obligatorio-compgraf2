#pragma once
#include "SceneObject.h"
class Cilinder :
    public SceneObject
{
private:
    double radius;
    double height;
public:
    Cilinder(double aRadius, double aHeigth, glm::dvec3 aPos)
    {
        radius = aRadius;
        height = aHeigth;
        position = aPos;
    }
    CollisionPoint* intersects(const Ray& ray);
};

