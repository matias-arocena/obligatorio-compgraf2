#pragma once
#include <glm/glm.hpp>

class Ray;

class SceneObject;

struct CollisionPoint {
	glm::vec3 normal;
	glm::vec3 position;
	float angle;
	SceneObject* object;
};

class SceneObject
{
public:
	CollisionPoint* intersects(const Ray& ray);
};

