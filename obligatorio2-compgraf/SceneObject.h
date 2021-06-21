#pragma once
#include <glm/glm.hpp>
#include "Color.h"

class Ray;

class SceneObject;

struct CollisionPoint {
	glm::dvec3 normal;
	glm::dvec3 position;
	glm::dvec3 hitDir;
	double angle;
	SceneObject* object;
	double distanceFromOrigin;
};

class SceneObject
{
protected:
	glm::dvec3 position;
	
public:
	Color ambient;
	Color specular;
	Color diffuse;
	double reflectionCoefficient;
	double transmissionCoefficient;
	double shininess;
	double alpha;

	bool solveQuadratic(const double& a, const double& b, const double& c, double& x0, double& x1);
	virtual CollisionPoint* intersects(const Ray& ray);
	double getReflectionCoefficient();
	double getShininess();
	double getTransmissionCoefficient();
	double getMedium();
};

