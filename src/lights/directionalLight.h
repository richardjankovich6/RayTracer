#pragma once

#include "vector3.h"
#include "color.h"
#include "light.h"

#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

using DirectionalLight = Light;

// struct DirectionalLight : private Light {

//     // Color lightColor;
//     // Color ambientLight;
//     // Color backgroundColor;

//     // Vector3<double> directionToLight;

//     DirectionalLight(Color lightColor, Vector3<double> directionToLight = {1,0,0}, Color ambientLight = {50, 50, 50}, Color backgroundColor = {20, 20, 20})
//     : lightColor(lightColor), ambientLight(ambientLight), backgroundColor(backgroundColor), directionToLight(directionToLight) {}
    
//     // DirectionalLight() {};

//     // DirectionalLight(const DirectionalLight &) = default;
//     // DirectionalLight(DirectionalLight &&) = default;
//     // DirectionalLight &operator=(const DirectionalLight &) = default;
//     // DirectionalLight &operator=(DirectionalLight &&) = default;

//     // inline Color getLightColor() const { return lightColor; }
//     // inline Color getAmbientLight() const { return ambientLight; }
//     // inline Color getBackgroundColor() const { return backgroundColor; }
//     // inline Vector3<double> getDirectionToLight() const { return directionToLight; }

// };

#endif