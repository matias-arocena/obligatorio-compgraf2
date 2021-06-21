#pragma once
#include <glm/glm.hpp>

class Ray
{
public:
	Ray(glm::dvec3 origin, glm::dvec3 direction, double currentMedium);
	glm::dvec3 getOrigin() const;
	glm::dvec3 getDirection() const;
	double getMedium() const;
private:
	double medium;
	glm::dvec3 origin;
	glm::dvec3 direction;
};

