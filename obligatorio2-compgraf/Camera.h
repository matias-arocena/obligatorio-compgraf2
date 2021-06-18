#pragma once
#include <glm/glm.hpp>
class Camera
{
public:
	Camera(glm::vec3 position, float near, float viewportHeight);
	glm::vec3 getPosition(); 
	glm::vec3 getDirectionToViewport(float u, float v);
	float getViewportHeight();
	float getViewportWidth();
private:
	float near;
	float viewportHeight;
	float viewportWidth;
	glm::vec3 viewportCorner;
	glm::vec3 position;
	glm::vec3 uAxis;
	glm::vec3 vAxis;
};

