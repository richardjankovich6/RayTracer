#pragma once

#include "vector3.h"
#include "color.h"
#include "light.h"
#include "point.h"

#ifndef POINTLIGHT_H
#define POINTLIGHT_H

// using DirectionalLight = Light;

struct PointLight : private Light {

    Point location;

    PointLight(Color lightColor, Point location)
    : location(location), Light(lightColor) {}

    virtual Vector3<double> getDirectionToLight(const Point from) const { return location - from; }


};

#endif