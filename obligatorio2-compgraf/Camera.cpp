#include "Camera.h"

#include "Constants.h"

Camera::Camera(glm::dvec3 position, double near, double viewportHeight) : position{ position }, near{ near }, viewportHeight{ viewportHeight }
{
    
    viewportWidth = ASPECT_RATIO * viewportHeight;
    uAxis = glm::dvec3(viewportWidth, 0.f, 0.f);
    vAxis = glm::dvec3(0.f, viewportHeight, 0.f);

    viewportCorner = position - uAxis / 2.0 - vAxis / 2.0 + glm::dvec3(0,0, near);
/*
	Vector3 t = s.camera.center - s.camera.eye;
	Vector3 b = s.camera.up.cross(t);

	Vector3 tn = t.normalize();
	Vector3 bn = b.normalize();
	Vector3 vn = tn.cross(bn);

	double gx = tan((s.camera.fov * M_PI / 180) / 2);
	double gy = gx * ((float)s.height / (float)s.width);

	Vector3 qx = (2 * gx * bn) / (s.width - 1);
	Vector3 qy = (2 * gy * vn) / (s.height - 1);

	Vector3 p1m = tn - gx * bn - gy * vn;

	*/
}

glm::dvec3 Camera::getPosition()
{
    return position;
}

glm::dvec3 Camera::getDirectionToViewport(double u, double v)
{
    return viewportCorner + u * uAxis + v * vAxis + position;
}

double Camera::getViewportHeight()
{
    return viewportHeight;
}

double Camera::getViewportWidth()
{
    return viewportWidth;
}
