#include "Ray.h"

Ray::Ray(glm::vec3 pixel, glm::vec3 projectionCenter) : pixel {pixel}, projectionCenter {projectionCenter}
{ 
	dir = glm::normalize(pixel - projectionCenter);
}

glm::vec3 Ray::getPixel() const
{
	return pixel;
}

glm::vec3 Ray::getProjectionCenter() const
{
	return projectionCenter;
}

glm::vec3 Ray::getDir() const 
{
	return dir;
}

