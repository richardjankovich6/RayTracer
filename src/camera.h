#pragma once

// #include <cmath>

#include "vector3.h"
#include "point.h"
// #include "ray.h"

#ifndef CAMERA_H
#define CAMERA_H

struct Camera {
public:

    Point lookAt;
    Point lookFrom;
    Vector3<double> lookUp;

    int width;
    int height;

    double fov;

    double widthExtent;
    double heightExtent;

    double halfPixelWidth;
    double halfPixelHeight;
};

#endif