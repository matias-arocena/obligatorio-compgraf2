#include "Sphere.h"
#include "Ray.h"
#include <iostream>

CollisionPoint* Sphere::intersects(const Ray& ray)
{
    float t0, t1; // solutions for t if the ray intersects 

    glm::vec3 L = ray.pixel - position;
    float a = glm::dot(ray.dir, ray.dir);
    float b = 2 * glm::dot(ray.dir, L);
    float c = glm::dot(L, L) - radius * radius;
    if (!solveQuadratic(a, b, c, t0, t1)) return nullptr;
    if (t0 > t1)
    {
        float aux = t0;
        t0 = t1;
        t1 = aux;
    }

    //std::cout << "collision check " << t0 << " " << t1 << std::endl;
    if (t0 < 0) {
        t0 = t1; // if t0 is negative, let's use t1 instead 
        if (t0 < 0) return nullptr; // both t0 and t1 are negative 
    }

    CollisionPoint* closestPoint = new CollisionPoint();
    closestPoint->position = ray.pixel + ray.dir * t0;
    closestPoint->normal = glm::normalize(closestPoint->position - position);
    closestPoint->hitDir = ray.dir;
    closestPoint->angle = std::asin(glm::dot(ray.dir, closestPoint->normal));
    closestPoint->object = this;

    return closestPoint;
}


