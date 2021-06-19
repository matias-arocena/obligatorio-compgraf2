#pragma once
#include <glm/glm.hpp>
#include "Color.h"

class Ray;

class SceneObject;

struct CollisionPoint {
	glm::vec3 normal;
	glm::vec3 position;
	glm::vec3 hitDir;
	float angle;
	SceneObject* object;
	float distanceFromOrigin;
};

class SceneObject
{
protected:
	glm::vec3 position;
	
public:
	Color ambient;
	Color specular;
	Color diffuse;
	float reflectionCoefficient;
	float transmissionCoefficient;
	float shininess;
	float alpha;

	bool solveQuadratic(const double& a, const double& b, const double& c, double& x0, double& x1);
	virtual CollisionPoint* intersects(const Ray& ray);
	float getReflectionCoefficient();
	float getShininess();
	float getTransmissionCoefficient();
	float getMedium();
};

