#pragma once

#include "vector3.h"
#include "point.h"

#ifndef RAY_H
#define RAY_H

class Ray {
public:
    Ray() {}
    Ray(const Point& origin, const Vector3<>& direction) : orig(origin), dir(direction) {}
    inline const Point& origin() const { return orig; }
    inline const Vector3<>& direction() const { return dir; }
    inline Point at(double val) const { return orig + val * dir; }

private:
    Point orig; // origin
    Vector3<> dir; // direection
};

Ray castRay(const Point& origin, const Vector3<>& direction) {
    return Ray(origin, direction);
}


#endif