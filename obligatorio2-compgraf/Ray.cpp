#include "Ray.h"

Ray::Ray(glm::dvec3 origin, glm::dvec3 direction, double medium) : 
	origin {origin}, direction {glm::normalize(direction)}, medium{medium}
{ 
}

glm::dvec3 Ray::getOrigin() const
{
	return origin;
}

glm::dvec3 Ray::getDirection() const
{
	return direction;
}

double Ray::getMedium() const
{
	return medium;
}


