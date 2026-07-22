#pragma once

#include "primitive.h"
#include "ray.h"
#include "vector3.h"
#include "color.h"


#ifndef SPHERE_H
#define SPHERE_H


class Sphere : public Primitive {

private:

    Vector3<double> center;
    double radius;

public:

    Sphere() : Primitive() {};

    Sphere(Vector3<double> center, double radius) : center(center), radius(radius), Primitive() {
        // Primitive::Primitive();
    }

    // Sphere(Vector3<double> center, double radius, double kDiffuse, double kSpecular, double kAmbient, double kGloss, Color diffuseColor, Color specularColor, Color ambientColor)
    // : center(center), radius(radius), Primitive(kDiffuse, kSpecular, kAmbient, kGloss, diffuseColor, specularColor, ambientColor) {
    //     // Primitive(kDiffuse, kSpecular, kAmbient, kGloss, diffuseColor, specularColor, ambientColor);
    // }

    Sphere(Vector3<double> Icenter, double Iradius, double IkDiffuse, double IkSpecular, double IkAmbient, double IkGloss, Color IdiffuseColor, Color IspecularColor, Color IambientColor) {
        center = Icenter;
        radius = Iradius;
        kDiffuse = IkDiffuse;
        kSpecular = IkSpecular;
        kGloss = IkGloss;
        diffuseColor = IdiffuseColor;
        specularColor = IspecularColor;
        ambientColor = IambientColor;
        // Primitive(IkDiffuse, IkSpecular, IkAmbient, IkGloss, IdiffuseColor, IspecularColor, IambientColor);
    }

    // Sphere(Vector3<double> center, double radius)
    // : center(center), radius(radius) {}
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

    virtual bool intersetRay(Vector3<double>& v, Ray ray) const override {

        Vector3 positionDifference = center - ray.origin();

        bool bInside = positionDifference.length() < radius;

        double tca = dot(ray.direction(), positionDifference);

        if (!bInside && (tca < 0)) {
            return false;
        }

        double thcSquare = pow(radius, 2) - positionDifference.lengthSquared() + pow(tca, 2);

        if (thcSquare < 0) {
            return false;
        }

        double t = tca;
        t += bInside ? sqrt(thcSquare) : -sqrt(thcSquare);

        v = ray.origin() + ray.direction() * t;
        return true;
    }

};

#endif