#pragma once

#include "color.h"
#include "light.h"
#include "vector3.h"
#include "ray.h"
#include <algorithm>
#include <cmath>

// class Ray;

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

class Primitive {

protected:

    double kDiffuse;
    double kSpecular;
    double kAmbient;
    double kGloss;

    Color diffuseColor;
    Color specularColor;
    Color ambientColor;

    Light light;

public:

    Primitive(double kDiffuse, double kSpecular, double kAmbient, double kGloss, Color diffuseColor, Color specularColor, Color ambientColor, Light light)
    : kDiffuse(kDiffuse), kSpecular(kSpecular), kAmbient(kAmbient), kGloss(kGloss), diffuseColor(diffuseColor), specularColor(specularColor), ambientColor(ambientColor), light(light) {}
    
    Primitive() {};
    virtual ~Primitive() = default;

    Color phongShading(Vector3<double> normal, Vector3<double> lookDirection) {
        
        Color aColor = light.getAmbientLight() * kAmbient * diffuseColor;

        double dotProduct = dot(normal, light.getDirectionToLight());
        Color dColor = kDiffuse * light.getLightColor() * diffuseColor * fmax(0.0, dotProduct);

        Vector3 reflectVector = 2 * normal * light.getDirectionToLight();
        // reflectVector = reflectVector / ( 2 * normal * dotProduct - light.getDirectionToLight());
        // reflectVector = 1.0 / ( 2 * normal * dotProduct - light.getDirectionToLight());
        reflectVector = unitVec(reflectVector);
        Color sColor = kSpecular * light.getLightColor() * specularColor * pow(fmax(0.0, dot(lookDirection, reflectVector)), kGloss);

        Color finalColor = aColor + sColor + dColor;
        return finalColor;
    }

    inline double getKDiffiuse() const {return kDiffuse;}
    inline void setKDiffiuse(double const val) {kDiffuse = val;}
    
    inline double getKSpecular() const {return kSpecular;}
    inline void setKSpecular(double const val) {kSpecular = val;}

    inline double getKAmbient() const {return kAmbient;}
    inline void setKAmbient(double const val) {kAmbient = val;}

    inline double getKGloss() const {return kGloss;}
    inline void setKGloss(double const val) {kGloss = val;}

    inline Color getDiffiuseColor() const {return diffuseColor;}
    inline void setDiffiuseColor(Color const val) {diffuseColor = val;}

    inline Color getSpecularColor() const {return specularColor;}
    inline void setSpecularColor(Color const val) {specularColor = val;}

    inline Color getAmbientColor() const {return ambientColor;}
    inline void setAmbientColor(Color const val) {ambientColor = val;}

    virtual bool intersetRay(Vector3<double>& v, Ray ray) const = 0;

    // virtual Vector3<double> intersetRay(Ray ray) const = 0;
};

#endif