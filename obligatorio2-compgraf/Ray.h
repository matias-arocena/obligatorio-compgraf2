#pragma once
#include <glm/glm.hpp>

class Ray
{
public:
	Ray(glm::vec2 pixel, glm::vec3 projectionCenter);
	glm::vec2 getPixel();
	glm::vec3 getProjectionCenter();

private:
	glm::vec2 pixel;
	glm::vec3 projectionCenter;

};

