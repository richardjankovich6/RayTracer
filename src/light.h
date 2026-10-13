#pragma once

#include "vector3.h"
#include "color.h"

#ifndef LIGHT_H
#define LIGHT_H

class Light {

    Color lightColor;
    Color ambientLight;
    Color backgroundColor;

    Vector3<double> directionToLight;

public:

    Light(Color lightColor, Vector3<double> directionToLight = {1,0,0}, Color ambientLight = {50, 50, 50}, Color backgroundColor = {20, 20, 20})
    : lightColor(lightColor), ambientLight(ambientLight), backgroundColor(backgroundColor), directionToLight(directionToLight) {}
    
    Light() {};

    Light(const Light &) = default;
    Light(Light &&) = default;
    Light &operator=(const Light &) = default;
    Light &operator=(Light &&) = default;

    Color getLightColor() { return lightColor; }
    Color getAmbientLight() { return ambientLight; }
    Color getBackgroundColor() { return backgroundColor; }
    Vector3<double> getDirectionToLight() { return directionToLight; }

};

#endif