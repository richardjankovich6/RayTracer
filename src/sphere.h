#pragma once

#include "primitive.h"
#include "ray.h"
#include "point.h"
#include "vector3.h"
#include "color.h"


#ifndef SPHERE_H
#define SPHERE_H


class Sphere : public Primitive {

private:

    Point center;
    double radius;

public:

    Sphere() : Primitive() {};

    Sphere(Vector3<double> center, double radius) : center(center), radius(radius), Primitive() {}

    Sphere(Vector3<double> Icenter, double Iradius, double IkDiffuse, double IkSpecular, double IkAmbient, double IkGloss, Color IdiffuseColor, Color IspecularColor, Color IambientColor) {
        center = Icenter;
        radius = Iradius;
        kDiffuse = IkDiffuse;
        kSpecular = IkSpecular;
        kGloss = IkGloss;
        diffuseColor = IdiffuseColor;
        specularColor = IspecularColor;
        ambientColor = IambientColor;
    }

    inline Vector3<> getReflectVec(Vector3<> normal, Vector3<> directionToLight) {
        return unitVec(2 * normal * dot(normal, directionToLight) - directionToLight);
    }

    Vector3<double> getCenter() const { return center; }
    void setCenter(Vector3<double> const val) { center = val; }

    double getRadius() const { return radius; }
    void setRadius(double const val) { radius = val; }

    bool intersetRay(Point& p, const Ray ray) const override {
        Vector3 oc = center - ray.origin();
        bool bInside = oc.length() < radius;

        double tca = dot(oc, ray.direction());

        if (!bInside && (tca < 0)) {
            // ray points away from sphere
            return false;
        }

        double thcSquare = pow(radius, 2) - oc.lengthSquared() + pow(tca, 2);
        if (thcSquare < 0) {
            return false;
        }

        double t = tca;
        t += bInside ? sqrt(thcSquare) : -sqrt(thcSquare);
        p = ray.origin() + ray.direction() * t;
        return true;
    }

    virtual Color getColor(Point p, Vector3<double> v, bool inShadow) const override {
        Vector3 normal = (p - center) / radius;
        return phongShading(normal, v, inShadow);
    }

    Vector3<double> getNormal(Point p) const override {
        return (p - center) / radius;
    }

};

#endif
