#pragma once

#include "../vector3.h"
#include "../point.h"
#include "../color.h"

#ifndef LIGHT_H
#define LIGHT_H

struct Light {

    static Color ambientLight;
    static Color backgroundColor;

    Color lightColor;
    double intensity;


    Light(Color lightColor, double intensity = 1.0)
    : lightColor(lightColor), intensity(intensity) {}
    
    Light() {};

    Light(const Light &) = default;
    Light(Light &&) = default;
    Light &operator=(const Light &) = default;
    Light &operator=(Light &&) = default;

    virtual Color getLightColor(const Point from = Point()) const { return lightColor; }
    virtual Vector3<double> getDirectionToLight(const Point from = Point()) const = 0;

};

#endif