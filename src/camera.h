#pragma once

#include <cmath>

#include "vector3.h"
#include "point.h"
#include "ray.h"

#ifndef CAMERA_H
#define CAMERA_H


class Camera {

private:
    Point lookAt;
    Point lookFrom;
    
    Vector3<double> lookUp;

    int width;
    int height;

    double fovWidth;
    double fovHeight;

    double xExtent;
    double yExtent;

    double hpx;
    double hpy;

public:

    Camera() {};

    Camera(Point lookAt, Point lookFrom, Vector3<double> lookUp, int width,
    int height, double fovWidth, double fovHeight, double xExtent,
    double yExtent, double hpx, double hpy)
    : lookAt(lookAt), lookFrom(lookFrom), lookUp(lookUp), width(width),
    height(height), fovWidth(fovWidth), fovHeight(fovHeight),
    xExtent(xExtent), yExtent(yExtent), hpx(hpx), hpy(hpy) {


        xExtent = fabs(tan(fovWidth / 2) * (lookAt - lookFrom).length());
        yExtent = fabs(tan(fovHeight / 2) * (lookAt - lookFrom).length() / (width / height));

        hpx = xExtent / width;
        hpy = yExtent / height;

    }
    
    Camera(const Camera &) = default;
    Camera(Camera &&) = default;
    Camera &operator=(const Camera &) = default;
    Camera &operator=(Camera &&) = default;

    Ray makeRay(int w, int h) {

        double x = xExtent + hpx * (w + 0.5);
        double y = yExtent + hpy * (h + 0.5);

        Vector3<double> direction = Vector3<double>(x, y, lookAt.z()) - lookFrom;

        return Ray(lookFrom, unitVec(direction));
    }
};

#endif