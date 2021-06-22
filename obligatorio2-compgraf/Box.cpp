#include "Box.h"

#include "Ray.h"

Box::Box(glm::dvec3 minVertex, glm::dvec3 maxVertex) : minVertex{minVertex}, maxVertex{maxVertex}
{
}

CollisionPoint* Box::intersects(const Ray& r)
{
    float tmin = (minVertex.x - r.getOrigin().x) / r.getDirection().x;
    float tmax = (maxVertex.x - r.getOrigin().x) / r.getDirection().x;

    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (minVertex.y - r.getOrigin().y) / r.getDirection().y;
    float tymax = (maxVertex.y - r.getOrigin().y) / r.getDirection().y;

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    float tzmin = (minVertex.z - r.getOrigin().z) / r.getDirection().z;
    float tzmax = (maxVertex.z - r.getOrigin().z) / r.getDirection().z;

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return nullptr;

    if (tzmin > tmin)
        tmin = tzmin;

    if (tzmax < tmax)
        tmax = tzmax;

    //TODO: Rellenar collision point
    CollisionPoint* collisionPoint = new CollisionPoint();
    return collisionPoint;
}