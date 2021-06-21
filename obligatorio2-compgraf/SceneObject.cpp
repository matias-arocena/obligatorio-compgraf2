#include "SceneObject.h"
#include "Ray.h"

CollisionPoint* SceneObject::intersects(const Ray& ray)
{
    return nullptr;
}

double SceneObject::getReflectionCoefficient()
{
    return reflectionCoefficient;
}

double SceneObject::getShininess()
{
    return shininess;
}

double SceneObject::getTransmissionCoefficient()
{
    return transmissionCoefficient;
}

double SceneObject::getMedium()
{
    return transmissionCoefficient;
}

bool SceneObject::solveQuadratic(const double& a, const double& b, const double& c, double& x0, double& x1)
{
    double discr = b * b - 4.0 * a * c;
    if (discr < 0) return false;
    else if (discr == 0) x0 = x1 = -0.5 * b / a;
    else {
        double q = (b > 0) ?
            -0.5 * (b + sqrt(discr)) :
            -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1)
    {
        double aux = x0;
        x0 = x1;
        x1 = aux;
    }

    return true;
}