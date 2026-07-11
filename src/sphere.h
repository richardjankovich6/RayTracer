#pragma once

#include "ray.h"
#include "vector3.h"
#include "color.h"

class Sphere {

    Vector3<double> center;
    double radius;
    double kDiffuse;
    double kSpecular;
    double kAmbient;
    double kGloss;

    Vector3<double> DiffuseColor;
    Vector3<double> SpecularColor;
    

public:
    Sphere(Vector3<> center, double radius, double kd, double ks, double ka, Vector3<> od, Vector3<> os, double kgls)
    : center(center), radius(radius), kDiffuse(kd), kSpecular(ks), kAmbient(ka), DiffuseColor(od), SpecularColor(os), kGloss(kgls) {};
    

    inline Vector3<> getReflectVec(Vector3<> normal, Vector3<> directionToLight) {
        return unitVec(2 * normal * dot(normal, directionToLight) - directionToLight);
        // double  dotProduct = dot(normal, directionToLight);
        // Vector3 reflectVec = 2 * normal * dotProduct - directionToLight;
        // return unitVec(reflectVec);
    }

    

    



    Color PhongShading();

    void intersetRay(Ray ray) {
        
    }


};