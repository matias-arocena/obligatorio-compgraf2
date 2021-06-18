#include "Ray.h"

Ray::Ray(glm::vec3 origin, glm::vec3 direction) : origin {origin}, direction {glm::normalize(direction)}
{ 
}

glm::vec3 Ray::getOrigin() const
{
	return origin;
}

glm::vec3 Ray::getDirection() const
{
	return direction;
}


