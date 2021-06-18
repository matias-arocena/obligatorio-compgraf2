#include "Camera.h"

#include "Constants.h"

Camera::Camera(glm::vec3 position, float near, float viewportHeight) : position{ position }, near{ near }, viewportHeight{ viewportHeight }
{
    viewportWidth = (SCREEN_HEIGHT / SCREEN_WIDTH) * viewportHeight;
    uAxis = glm::vec3(viewportWidth, 0.f, 0.f);
    vAxis = glm::vec3(0.f, viewportHeight, 0.f);
    viewportCorner = position - uAxis / 2.f - vAxis / 2.f + near;
}

glm::vec3 Camera::getPosition()
{
    return position;
}

glm::vec3 Camera::getDirectionToViewport(float u, float v)
{
    return viewportCorner + u * uAxis + v * vAxis + position;
}

float Camera::getViewportHeight()
{
    return viewportHeight;
}

float Camera::getViewportWidth()
{
    return viewportWidth;
}
