#include "Box.h"

#include "Ray.h"
#include "Plane.h"
#include <iostream>

Box::Box(glm::dvec3 aPosition, glm::dvec3 aUp, glm::dvec3 aFront, double aHeight, double aLength, double aWidth) : position{ aPosition }, up{ glm::normalize(aUp) }, front{ glm::normalize(aFront) }, height{ aHeight }, length{ aLength }, width{ aWidth }
{
    side = glm::normalize(glm::dvec3(up.y * front.z - up.z * front.y, up.z * front.x - up.x * front.z, up.x * front.y - up.y * front.x));
    std::cout << "side " << side.x << " " << side.y << " " << side.z << std::endl;

    planes.push_back(new Plane(up, position + up * height / 2., front, length, width)); //top
    planes.push_back(new Plane(-up, position - up * height / 2., front, length, width)); //bot 
    planes.push_back(new Plane(side, position + side * length / 2., up, width, height)); //right
    planes.push_back(new Plane(-side, position - side * length / 2., up, width, height)); //left
    planes.push_back(new Plane(front, position + front * width / 2., up, length, height)); //front
    planes.push_back(new Plane(-front, position - front * width / 2., up, length, height)); //back
}

void Box::setData(glm::vec3 aDiffuse, glm::vec3 aAmbient, glm::vec3 aSpecular, float aShininess, float aAlpha, float aReflectionCoefficient, float aTransmissionCoefficient)
{
    for (SceneObject* obj : planes)
    {
        obj->diffuse = Color();
        obj->diffuse.rgb = aDiffuse;
        obj->ambient = Color();
        obj->ambient.rgb = aAmbient;
        obj->specular = Color();
        obj->specular.rgb = aSpecular;
        obj->shininess = aShininess;
        obj->alpha = aAlpha;
        obj->reflectionCoefficient = aReflectionCoefficient;
        obj->transmissionCoefficient = aTransmissionCoefficient;
    }
    
}

CollisionPoint* Box::intersects(const Ray& r)
{
    CollisionPoint* closest = nullptr;
    for (SceneObject* obj : planes)
    {
        CollisionPoint* candidate = obj->intersects(r);
        if (candidate != nullptr && (closest == nullptr || candidate->distanceFromOrigin < closest->distanceFromOrigin))
            closest = candidate;
    }
    return closest;
}