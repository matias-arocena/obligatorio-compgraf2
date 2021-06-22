#include "Cilinder.h"
#include "Ray.h"
#include "Constants.h"
#include <iostream>

CollisionPoint* Cilinder::intersects(const Ray& ray)
{
	// Arranco por buscar la intersección entre la recta y la circunsferencia en el plano x,z

	// Componentes del vector dirección del rayo
	float x_d = ray.getDirection().x;
	float z_d = ray.getDirection().z;

	// Origen del rayo
	float x_o = ray.getOrigin().x;
	float z_o = ray.getOrigin().z;

	// Centro de la circunferencia
	float x_c = position.x;
	float z_c = position.z;

	// Resuelvo la ecuación cuadrática para encontrar la posición de las intersecciones en relación al rayo.
	float a = std::pow(x_d, 2) + std::pow(z_d, 2);
	float b = 2 * (x_d * (x_o - x_c) + z_d * (z_o - z_c));
	float c = std::pow(x_o - x_c, 2) + std::pow(z_o - z_c, 2) - std::pow(radius, 2);

    // Check if a solution exists
    if (b * b - 4 * a * c > 0) 
    {
        glm::dvec3 normal = glm::dvec3(0, 0, 0);

        double t0, t1;
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

        glm::vec3 candidate = ray.getOrigin() + ray.getDirection() * t0;
        if (candidate.y < position.y || candidate.y > position.y + height)
        {
            t0 = -1; // set as negative to check if ray doesnt collide with bottom or top

            float y_o = ray.getOrigin().y;
            float y_d = ray.getDirection().y;
            float y_f; // y position of the face
            // If the intersection with the circunference is too high or too low, check with the top and bottom faces of the cylinder.
            if (ray.getOrigin().y < position.y)
            {
                y_f = position.y;
                double possible_t = (y_f - y_o) / y_d;
                if (glm::distance(position, ray.getOrigin() + ray.getDirection() * possible_t) < radius)
                {
                    t0 = possible_t;
                    normal.y = -1;
                }
            }
            else if (ray.getOrigin().y > position.y + height)
            {
                y_f = position.y + height;
                double possible_t = (y_f - y_o) / y_d;
                if (glm::distance(position + glm::dvec3(0, height, 0), ray.getOrigin() + ray.getDirection() * possible_t) < radius)
                {
                    t0 = possible_t;
                    normal.y = 1;
                }
            }

            if (t0 < 0) return nullptr;
        }


        CollisionPoint* closestPoint = new CollisionPoint();

        closestPoint->position = ray.getOrigin() + ray.getDirection() * t0;

        if (glm::length(normal) == 0)
            normal = glm::normalize(closestPoint->position - glm::dvec3(position.x, closestPoint->position.y, position.z));
        closestPoint->normal = normal;

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