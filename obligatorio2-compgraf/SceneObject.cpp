#include "SceneObject.h"
#include "Ray.h"

CollisionPoint* SceneObject::intersects(const Ray& ray)
{
    return nullptr;
}

float SceneObject::getReflectionCoefficient()
{
    return reflectionCoefficient;
}

float SceneObject::getTransmissionCoefficient()
{
    return transmissionCoefficient;
}

bool SceneObject::solveQuadratic(const float& a, const float& b, const float& c, float& x0, float& x1)
{
    float discr = b * b - 4 * a * c;
    if (discr < 0) return false;
    else if (discr == 0) x0 = x1 = -0.5 * b / a;
    else {
        float q = (b > 0) ?
            -0.5 * (b + sqrt(discr)) :
            -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1)
    {
        float aux = x0;
        x0 = x1;
        x1 = aux;
    }

    return true;
}