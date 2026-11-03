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

    PointLight(Color lightColor, Point location, double intensity = 1.0)
    : location(location), intensity(intensity) Light(lightColor) {}

    virtual Vector3<double> getDirectionToLight(const Point from) const { return normalize(location - from); }

    virtual Color getLightColor(const Point from) {
        Vector3<double> dif = from - location;
        return lightColor * intensity / dif.lengthSquared();
    }


};

#endif