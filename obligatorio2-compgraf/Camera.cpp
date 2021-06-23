#include "Camera.h"

#include "Constants.h"

#define _USE_MATH_DEFINES
#include <math.h>

Camera::Camera(glm::dvec3 position, glm::dvec3 lookAt, glm::dvec3 vup, double fov, double near, double viewportHeight) :
	position{ position }, lookAt{ lookAt }, vup{ vup }, fov{ fov }
{
    
	glm::dvec3 t = lookAt - position;
	glm::dvec3 cross = glm::cross(vup, t);
	glm::dvec3 b = glm::normalize(glm::cross(vup, t));
	t = glm::normalize(t);

	double gx = std::tan((fov * M_PI / 180) / 2);
	double gy = gx * (SCREEN_HEIGHT / SCREEN_HEIGHT);

	uAxis = (2 * gx * b) / static_cast<double>(SCREEN_WIDTH - 1);
	vAxis = (2 * gy * glm::normalize(glm::cross(t, b))) / static_cast<double>(SCREEN_WIDTH - 1);

	viewportCorner = t - gx * b - gy * glm::normalize(glm::cross(t, b));
}

glm::dvec3 Camera::getPosition()
{
    return position;
}

glm::dvec3 Camera::getDirectionToViewport(double u, double v)
{
    return viewportCorner + u * uAxis + v * vAxis;
}

double Camera::getViewportHeight()
{
    return viewportHeight;
}

double Camera::getViewportWidth()
{
    return viewportWidth;
}
