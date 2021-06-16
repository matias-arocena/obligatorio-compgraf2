#pragma once
#include <glm/glm.hpp>

class Ray
{
public:
	Ray(glm::vec3 pixel, glm::vec3 projectionCenter);
	glm::vec3 getPixel() const;
	glm::vec3 getProjectionCenter() const;
	glm::vec3 getDir() const;
	void setDir(glm::vec3 dir);
	void setProjectionCenter(glm::vec3 projectionCenter);
	void setPixel(glm::vec3 pixel);
private:
	glm::vec3 pixel;
	glm::vec3 dir;
	glm::vec3 projectionCenter;
};

