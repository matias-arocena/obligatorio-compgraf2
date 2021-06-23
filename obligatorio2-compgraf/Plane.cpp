#include "Plane.h"

#include "Ray.h";
#include <iostream>

Plane::Plane(glm::dvec3 normal, glm::dvec3 point, glm::dvec3 up, double length, double height) : normal{ glm::normalize(normal) }, point{ point }, up{ normalize(up) }, length{ length }, height{ height }
{
    side = glm::normalize(glm::dvec3(up.y * normal.z - up.z * normal.y, up.z * normal.x - up.x * normal.z, up.x * normal.y - up.y * normal.x));
}

CollisionPoint* Plane::intersects(const Ray& ray)
{
    double denominator = glm::dot(normal, ray.getDirection());
    if (denominator > 0.000001) {
        glm::dvec3 p = point - ray.getOrigin();
        double t = glm::dot(p, normal) / denominator;

        glm::dvec3 candidate = ray.getOrigin() + t * ray.getDirection();

        //std::cout << "real dist " << glm::length(candidate - point) << std::endl;
        //Check if it's within height
        double dist = glm::length(glm::dot(candidate - point, up) / glm::dot(up, up) * up);
        //std::cout << "up " << dist << std::endl;
        if (dist > height / 2)
            return nullptr;

        //Check if it's within length
        dist = glm::length(glm::dot(candidate - point, side) / glm::dot(side, side) * side);
        //std::cout << "side " << dist << std::endl;
        if (dist > length / 2)
            return nullptr;

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
