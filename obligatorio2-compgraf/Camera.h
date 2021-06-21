#pragma once
#include <glm/glm.hpp>
class Camera
{
public:
	Camera(glm::dvec3 position, double near, double viewportHeight);
	glm::dvec3 getPosition(); 
	glm::dvec3 getDirectionToViewport(double u, double v);
	double getViewportHeight();
	double getViewportWidth();
private:
	double near;
	double viewportHeight;
	double viewportWidth;
	glm::dvec3 viewportCorner;
	glm::dvec3 position;
	glm::dvec3 uAxis;
	glm::dvec3 vAxis;
};

