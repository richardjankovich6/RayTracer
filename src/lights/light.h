#pragma once

#include "vector3.h"
#include "point.h"
#include "color.h"

#ifndef LIGHT_H
#define LIGHT_H

struct Light {

    static Color ambientLight;
    static Color backgroundColor;

    Color lightColor;


    Light(Color lightColor)
    : lightColor(lightColor) {}
    
    Light() {};

    Light(const Light &) = default;
    Light(Light &&) = default;
    Light &operator=(const Light &) = default;
    Light &operator=(Light &&) = default;

    inline Color getLightColor() const { return lightColor; }
    inline Color getAmbientLight() const { return ambientLight; }
    inline Color getBackgroundColor() const { return backgroundColor; }
    virtual Vector3<double> getDirectionToLight(const Point from = Point()) const = 0;

};

// Light::backgroundColor = Color(0.0, 0.0, 0.0);

#endif