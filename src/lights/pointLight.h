#pragma once

#include "vector3.h"
#include "color.h"
#include "light.h"
#include "point.h"

#ifndef POINTLIGHT_H
#define POINTLIGHT_H

// using DirectionalLight = Light;

struct PointLight : Light {

    Point location;

    double intensity;

    PointLight(Color lightColor, Point location, double intensity = 1)
    : location(location), Light(lightColor) {}

    virtual Vector3<double> getDirectionToLight(const Point from) const { return location - from; }

    virtual Color getLightColor(const Point from) {
        Vector3<double> dif = from - location;
        return lightColor * intensity / dif.lengthSquared();
    }


};

#endif