#pragma once

#include "color.h"
#include "vector3.h"

class Ray;


class Primitive {

protected:

    double kDiffuse;
    double kSpecular;
    double kAmbient;
    double kGloss;

    Color diffuseColor;
    Color specularColor;
    Color ambientColor;

public:
    Primitive(double kDiffuse, double kSpecular, double kAmbient, double kGloss, Color diffuseColor, Color specularColor, Color ambientColor)
    : kDiffuse(kDiffuse), kSpecular(kSpecular), kAmbient(kAmbient), kGloss(kGloss), diffuseColor(diffuseColor), specularColor(specularColor), ambientColor(ambientColor) {}
    
    Primitive() {};

    Color phongShading(Vector3<double> normal, Vector3<double> lookDirection) {
        return Color();
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

    virtual Vector3<double> intersetRay(Ray ray) const;
};