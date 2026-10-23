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

    bool doubleSided;

public:

    Triangle(Point p1, Point p2, Point p3) : p1(p1), p2(p2), p3(p3) {
        Vector3<double> v1 = p3 - p2;
        Vector3<double> v2 = p1 - p2;

        normal = unitVec(cross(v1, v2));

        // this is a fucking thing
        d = -(p1[0] * normal[0] + p1[1] * normal[1] + p1[2] * normal[2]);


        doubleSided = true;
    };

    bool intersetRay(Point& p, const Ray ray) const override {
        double vd = dot(normal, ray.direction());
        if (vd == 0) {
            return false;
        }
        if (!doubleSided && vd) {
            return false;
        } 

        double vo = -(dot(normal, ray.direction()) + d);
        double t = vo / vd;
        
        if (t < 0) {
            return false;
        }

        p = ray.origin() + ray.direction() * t;

        bool dot1 = dot(normal, cross(p2 - p1, p - p1)) > 0;
        bool dot2 = dot(normal, cross(p3 - p2, p - p2)) > 0;
        bool dot3 = dot(normal, cross(p1 - p3, p - p3)) > 0;

        bool interset = dot1 && dot2 && dot3;

        return interset;

        // if (vd > 0 && doubleSided) {
        //     auto tempNormal = -normal;
        //     bool dot1 = dot(-tempNormal, cross(p2 - p1, p - p1)) > 0;
        //     bool dot2 = dot(-tempNormal, cross(p3 - p2, p - p2)) > 0;
        //     bool dot3 = dot(-tempNormal, cross(p1 - p3, p - p3)) > 0;
        //     bool interset = dot1 && dot2 && dot3;
        // }
        // else {
        //     bool dot1 = dot(normal, cross(p2 - p1, p - p1)) > 0;
        //     bool dot2 = dot(normal, cross(p3 - p2, p - p2)) > 0;
        //     bool dot3 = dot(normal, cross(p1 - p3, p - p3)) > 0;
        //     bool interset = dot1 && dot2 && dot3;
        // }
        // return interset;
    }

    virtual Color getColor(Point p, Vector3<double> v, bool inShadow) const override {
        return phongShading(normal, v, inShadow);
    }
    
};

#endif