#include "Plane.h"

#include "Ray.h";

Plane::Plane(glm::dvec3 normal, glm::dvec3 point) : normal {glm::normalize(normal)}, point {point}
{
}

CollisionPoint* Plane::intersects(const Ray& ray)
{
    double denominator = glm::dot(normal, ray.getDirection());
    if (denominator > 0.000001) {
        glm::dvec3 p = point - ray.getOrigin();
        double t = glm::dot(p, normal) / denominator;
        if (t >= 0) {
            CollisionPoint* collisionPoint = new CollisionPoint();
            collisionPoint->normal = normal;
            collisionPoint->position = ray.getOrigin() + t * ray.getDirection();
            collisionPoint->hitDir = ray.getDirection();
            collisionPoint->angle = std::asin(glm::dot(ray.getDirection(), normal));
            collisionPoint->object = this;
            collisionPoint->distanceFromOrigin = t;
            return collisionPoint;
        }
    }
    return nullptr;
}
