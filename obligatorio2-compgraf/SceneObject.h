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
};

class SceneObject
{
protected:
	glm::vec3 position;
	float reflectionCoefficient;
	float transmissionCoefficient;
public:
	Color color;
	bool solveQuadratic(const float& a, const float& b, const float& c, float& x0, float& x1);
	virtual CollisionPoint* intersects(const Ray& ray);
	float getReflectionCoefficient();
	float getTransmissionCoefficient();
};

