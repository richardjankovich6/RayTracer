#include "primitive.h"

Color Primitive::phongShading(Vector3<double> normal, Vector3<double> lookDirection, bool inShadow) const {
    Color aColor = light->getAmbientLight() * kValues.z() * diffuseColor;
    if (inShadow) {
        return aColor;
    }

    double dotProduct = dot(normal, light->getDirectionToLight());
    Color dColor = kValues.x() * light->getLightColor() * diffuseColor * fmax(0.0, dotProduct);

    Vector3 reflectVector = 2 * normal * light->getDirectionToLight();
    // reflectVector = reflectVector / ( 2 * normal * dotProduct - light->getDirectionToLight());
    // reflectVector = 1.0 / ( 2 * normal * dotProduct - light->getDirectionToLight());
    reflectVector = unitVec(reflectVector);
    Color sColor = kValues.y() * light->getLightColor() * specularColor * pow(fmax(0.0, dot(lookDirection, reflectVector)), kGloss);

    return aColor + sColor + dColor;
}