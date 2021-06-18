#pragma once
#include <glm/glm.hpp>

class Ray
{
public:
	Ray(glm::vec3 origin, glm::vec3 direction);
	glm::vec3 getOrigin() const;
	glm::vec3 getDirection() const;
private:
	glm::vec3 origin;
	glm::vec3 direction;
};

