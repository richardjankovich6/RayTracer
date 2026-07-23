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
    int height, double fovWidth, double fovHeight)
    : lookAt(lookAt), lookFrom(lookFrom), lookUp(lookUp), width(width),
    height(height), fovWidth(fovWidth), fovHeight(fovHeight) {

        // convert to radians
        double fw = fovWidth * M_PI / 180.0;
        double fh = fovHeight * M_PI / 180.0;
        // xExtent = fabs(tan(fw / 2) * (lookAt - lookFrom).length());
        xExtent = std::abs(tan(fw / 2) * (lookAt - lookFrom).length());
        // yExtent = fabs(tan(fh / 2) * (lookAt - lookFrom).length() / ((float)width / (float)height));
        // yExtent = fabs(tan(fh / 2) * (lookAt - lookFrom).length() * static_cast<double>(height) / static_cast<double>(width));
        yExtent = std::abs(tan(fh / 2) * (lookAt - lookFrom).length() * static_cast<double>(height) / static_cast<double>(width));

        hpx = xExtent / width;
        hpy = yExtent / height;
    }
    
    Camera(const Camera &) = default;
    Camera(Camera &&) = default;
    Camera &operator=(const Camera &) = default;
    Camera &operator=(Camera &&) = default;

    Ray makeRay(int w, int h) {

        double x = xExtent + hpx * (static_cast<double>(w) + 0.5);
        double y = yExtent + hpy * (static_cast<double>(h) + 0.5);

        Vector3<double> direction = Vector3<double>(x, y, lookAt.z()) - lookFrom;

        return Ray(lookFrom, unitVec(direction));
    }
};

#endif