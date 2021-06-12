#pragma once
#include <glm/glm.hpp>

class Ray
{
public:
	Ray(glm::vec3 pixel, glm::vec3 projectionCenter);
	glm::vec3 getPixel();
	glm::vec3 getProjectionCenter();
	glm::vec3 pixel;
	glm::vec3 dir;
	glm::vec3 projectionCenter;

private:

};

