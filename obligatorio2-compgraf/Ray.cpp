#include "Ray.h"

Ray::Ray(glm::vec2 pixel, glm::vec3 projectionCenter) : pixel {pixel}, projectionCenter {projectionCenter}
{ 
}

glm::vec2 Ray::getPixel()
{
	return pixel;
}

glm::vec3 Ray::getProjectionCenter()
{
	return projectionCenter;
}


