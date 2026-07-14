#pragma once

#include "primitive.h"
#include "ray.h"
#include "vector3.h"
#include "color.h"

class Sphere : public Primitive {

    Vector3<double> center;
    double radius;

public:

    Sphere() {};

    Sphere(Vector3<double> center, double radius)
    : center(center), radius(radius) {}
    // Sphere(Vector3<> center, double radius, double kd, double ks, double ka,
    // Vector3<> od, Vector3<> os, double kgls) : center(center),
    // radius(radius), kDiffuse(kd), kSpecular(ks), kAmbient(ka),
    // DiffuseColor(od), SpecularColor(os), kGloss(kgls) {};

    inline Vector3<> getReflectVec(Vector3<> normal, Vector3<> directionToLight) {
        return unitVec(2 * normal * dot(normal, directionToLight) - directionToLight);
        // double  dotProduct = dot(normal, directionToLight);
        // Vector3 reflectVec = 2 * normal * dotProduct - directionToLight;
        // return unitVec(reflectVec);
    }

    Vector3<double> getCenter() const { return center; }
    void setCenter(Vector3<double> const val) { center = val; }

    double getRadius() const { return radius; }
    void setRadius(double const val) { radius = val; }

    void intersetRay(Ray ray) {

    }


};