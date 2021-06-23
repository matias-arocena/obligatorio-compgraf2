#pragma once
#include "SceneObject.h"
#include <vector>
class Box :
    public SceneObject
{
public:
    Box(glm::dvec3 position, glm::dvec3 up, glm::dvec3 front, double height, double length, double width);
    CollisionPoint* intersects(const Ray& ray);
    void setData(glm::vec3 aDiffuse, glm::vec3 aAmbient, glm::vec3 aSpecular, float aShininess, float aAlpha, float aReflectionCoefficient, float aTransmissionCoefficient);
private:
    glm::dvec3 position;
    glm::dvec3 up;
    glm::dvec3 front;
    glm::dvec3 side;
    double height;
    double length;
    double width;
    std::vector<SceneObject*> planes;
};

