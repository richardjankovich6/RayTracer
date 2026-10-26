#pragma once

#include "color.h"
#include "light.h"
#include "vector3.h"
#include "ray.h"
// #include <algorithm>
// #include <cmath>

// class Ray;

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

class Primitive {

protected:


    // diffuse is x, speculat is y, ambient is z
    Vector3<float> kValues;
    float kGloss;
    float reflectiveness;

    Color diffuseColor;
    Color specularColor;
    Color ambientColor;

    Light light;

    inline void setKValues(float d, float s, float a) {
        kValues = unitVec<float>({d, s, a});
    }

public:

    Primitive(double kDiffuse, double kSpecular, double kAmbient, double kGloss, double reflectiveness, Color diffuseColor, Color specularColor, Color ambientColor, Light light)
    : kGloss(kGloss), reflectiveness(reflectiveness), diffuseColor(diffuseColor), specularColor(specularColor), ambientColor(ambientColor), light(light) {
        setKValues(kDiffuse, kSpecular, kAmbient);
    }
    
    Primitive() {};
    virtual ~Primitive() = default;

    Color phongShading(Vector3<double> normal, Vector3<double> lookDirection, bool inShadow) const;

    inline double getKDiffiuse() const { return kValues.x(); }
    inline void setKDiffiuse(double const val) { setKValues(val, kValues.y(), kValues.z()); }
    
    inline double getKSpecular() const { return kValues.y(); }
    inline void setKSpecular(double const val) { setKValues(kValues.x(), val, kValues.z()); }

    inline double getKAmbient() const { return kValues.z(); }
    inline void setKAmbient(double const val) { setKValues(kValues.x(), kValues.y(), val); }

    inline double getKGloss() const { return kGloss; }
    inline void setKGloss(double const val) { kGloss = val; }

    inline double getReflectiveness() const { return reflectiveness; }
    inline void setReflectiveness(double const val) { reflectiveness = val; }

    inline Color getDiffiuseColor() const { return diffuseColor; }
    inline void setDiffiuseColor(Color const val) { diffuseColor = val; }

    inline Color getSpecularColor() const { return specularColor; }
    inline void setSpecularColor(Color const val) { specularColor = val; }

    inline Color getAmbientColor() const { return ambientColor; }
    inline void setAmbientColor(Color const val) { ambientColor = val; }

    inline Color getBackgroundColor() const { return light.getBackgroundColor(); }

    inline void setLight(const Light l) { light = l; }

    

    // virtual bool intersetRay(Point& v, const Ray ray) const {
    //     return false;
    // };
    virtual bool intersetRay(Point& v, Ray ray) const = 0;

    virtual Color getColor(Point p, Vector3<double> v, bool inShadow) const {
        return getBackgroundColor();
    }

    virtual Vector3<double> getNormal(Point p) const {
        return Vector3<double>();
    }

    // virtual Vector3<double> intersetRay(Ray ray) const = 0;
};

#endif