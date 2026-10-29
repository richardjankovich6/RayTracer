#pragma once

#include "vector3.h"
#include "color.h"
#include "light.h"

#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

struct DirectionalLight : Light {

    Vector3<double> directionToLight;

    DirectionalLight(Color lightColor, Vector3<double> directionToLight = {1,0,0})
    : directionToLight(directionToLight), Light(lightColor) {}


    virtual Vector3<double> getDirectionToLight(const Point ignored) const { return directionToLight; }

};



#endif