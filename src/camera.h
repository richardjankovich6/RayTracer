#pragma once

#include <cmath>

#include "vector3.h"
#include "point.h"
#include "ray.h"

#ifndef CAMERA_H
#define CAMERA_H

struct Camera {

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

    Camera(Point lookAt, Point lookFrom, Vector3<double> lookUp, 
        int width, int height, double fov)
    : lookAt(lookAt), lookFrom(lookFrom), lookUp(lookUp), 
    width(width), height(height), fov(fov)
    {    
        double magnitude = (lookAt - lookFrom).length();

        double aspectRatio = static_cast<double>(width) / static_cast<double>(height);
        double tangentHeight = tan(fov / aspectRatio / 2 * M_PI / 180.0);
        double tangentWidth = tan(fov / 2 * M_PI / 180.0);

        widthExtent = std::abs(tangentWidth * magnitude);
        heightExtent = std::abs(tangentHeight * magnitude);

        halfPixelWidth = widthExtent / width;
        halfPixelHeight = heightExtent / height;
    }

    Ray makeRay(int w, int h) {
        double x = -widthExtent + halfPixelWidth * (2 * static_cast<double>(w) + 1.0);
        double y = heightExtent - halfPixelHeight * (2 * static_cast<double>(h) + 1.0);

        Vector3<double> direction = Vector3<double>(x, y, lookAt.z()) - lookFrom;

        return Ray(lookFrom, unitVec(direction));
    }
};

#endif