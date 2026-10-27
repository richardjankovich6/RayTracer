#pragma once

#include "vector3.h"
#include "point.h"
#include "primitive.h"

#ifndef TRIANGLE_H
#define TRIANGLE_H

class Triangle : public Primitive {

    Point p1;
    Point p2;
    Point p3;
    
    Vector3<double> normal;

    double d;

    bool doubleSided = false;

public:

    Triangle() {};

    void setNormal(const Vector3<double> v);

    Triangle(Point p1, Point p2, Point p3) : p1(p1), p2(p2), p3(p3) {
        findD();
    };

    void setP1(Point p) { p1 = p; }
    void setP2(Point p) { p2 = p; }
    void setP3(Point p) { p3 = p; }
    void setDoubleSided(bool b) { doubleSided = b; }

    // IMPORTANT
    // call this after assigning all 3 points
    void findD();

    bool intersetRay(Point& p, const Ray ray) const override;

    virtual Color getColor(Point p, Vector3<double> v, bool inShadow) const override {
        return phongShading(normal, v, inShadow);
    }
    
};

#endif