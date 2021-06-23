#pragma once
#include <glm/glm.hpp>
class Camera
{
public:
	Camera(glm::dvec3 position, glm::dvec3 lookAt, glm::dvec3 vup, double fov);
	glm::dvec3 getPosition(); 
	glm::dvec3 getDirectionToViewport(double u, double v);

private:
	double fov;
	glm::dvec3 position;
	glm::dvec3 lookAt;
	glm::dvec3 vup;
	glm::dvec3 uAxis;
	glm::dvec3 vAxis;
	glm::dvec3 viewportCorner;
};

