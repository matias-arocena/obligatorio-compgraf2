#include "Camera.h"

Camera::Camera(glm::vec3 position)
{
    this->position = position;
}

glm::vec3 Camera::getPosition()
{
    return position;
}
