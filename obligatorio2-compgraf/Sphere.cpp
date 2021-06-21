#include "Sphere.h"
#include "Ray.h"
#include <iostream>
#include "Constants.h"


CollisionPoint* Sphere::intersects(const Ray& ray)
{
    double t0, t1; // solutions for t if the ray intersects 
    glm::dvec3 L = ray.getOrigin() - position;
    double a = glm::dot(ray.getDirection(), ray.getDirection());
    double b = 2 * glm::dot(L, ray.getDirection());
    double c = glm::dot(L, L) - radius * radius;
    if (b * b - 4 * a * c > 0) {
        
        if (!solveQuadratic(a, b, c, t0, t1)) return nullptr;
        if (t0 > t1)
        {
            double aux = t0;
            t0 = t1;
            t1 = aux;
        }

        if (t0 < 0) {
            t0 = t1; // if t0 is negative, let's use t1 instead
            if (t0 < 0) return nullptr; // both t0 and t1 are negative
        }
        
        CollisionPoint* closestPoint = new CollisionPoint();

        closestPoint->position = ray.getOrigin() + ray.getDirection() * t0 ;
        closestPoint->normal = glm::normalize(closestPoint->position - position);
        closestPoint->position = closestPoint->position + closestPoint->normal * CORRECTION;
        closestPoint->hitDir = ray.getDirection();
        closestPoint->angle = std::asin(glm::dot(ray.getDirection(), closestPoint->normal));
        closestPoint->object = this;
        closestPoint->distanceFromOrigin = t0;

        return closestPoint;
        
    }
    else {
        return nullptr;
    }
}


