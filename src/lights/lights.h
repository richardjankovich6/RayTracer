#pragma once
#include <vector>

#include "../vector3.h"
#include "../point.h"
#include "../color.h"

#ifndef LIGHTS_H
#define LIGHTS_H

#define numLightValues 4
enum LightValueOffset {
    lightR = 0,
    lightG = 1,
    lightB = 2,
    lightIntensity = 3
};

struct Lights {

    static Color ambientLight;
    static Color backgroundColor;

    std::vector<double> lightValues;
    int numLights;
    Lights() {};

    void initializeVectors(int size) {
        numLights= size;
        lightValues = std::vector<double>(size * numLightValues);
    }
    void setLightColor(int i, Color lightColor, double intensity) {
        lightValues[i * numLightValues + LightValueOffset::lightR] = lightColor.x();
        lightValues[i * numLightValues + LightValueOffset::lightG] = lightColor.y();
        lightValues[i * numLightValues + LightValueOffset::lightB] = lightColor.z();
        lightValues[i * numLightValues + LightValueOffset::lightIntensity] = intensity;
    }

    Color getColor(int i) const { 
        return {
            lightValues[i * numLightValues + LightValueOffset::lightR],
            lightValues[i * numLightValues + LightValueOffset::lightG],
            lightValues[i * numLightValues + LightValueOffset::lightB]
        };
    }
    double getLightIntensity(int i) const {
        return lightValues[i * numLightValues + LightValueOffset::lightIntensity];
    }
    virtual Vector3<double> getDirectionToLight(const int i, const Point from = Point()) const = 0;

};

#endif

#ifndef POINTLIGHTS_H
#define POINTLIGHTS_H

// using DirectionalLight = Light;

#define numLightLocationValues 3
enum LightLocationValueOffset {
    locX = 0,
    locY = 1,
    locZ = 2
};


struct PointLights : Lights {

    std::vector<double> locations;

    PointLights() {};
    void initializeVectors(int size) {
        locations = std::vector<double>(size * numLightLocationValues);
        Lights::initializeVectors(size);
    }

    void setLight(int i, Vector3<double> location, Color lightColor, double intensity) {
        locations[i * numLightLocationValues + LightLocationValueOffset::locX] = location.x();
        locations[i * numLightLocationValues + LightLocationValueOffset::locY] = location.y();
        locations[i * numLightLocationValues + LightLocationValueOffset::locZ] = location.z();
        setLightColor(i, lightColor, intensity);
    }

    Vector3<double> getLightLocation(int i) const {
        return {
            locations[i * numLightLocationValues + LightLocationValueOffset::locX],
            locations[i * numLightLocationValues + LightLocationValueOffset::locY],
            locations[i * numLightLocationValues + LightLocationValueOffset::locZ]
        };
    }

    virtual Vector3<double> getDirectionToLight(int i, const Point from) const override { return normalize(getLightLocation(i) - from); }

    Color getLightColor(const int i, const Point from) const {
        Vector3<double> dif = from - getLightLocation(i);
        return Lights::getColor(i) * getLightIntensity(i) / dif.lengthSquared();
    }
};

#endif

#ifndef DIRECTIONALLIGHTS_H
#define DIRECTIONALLIGHTS_H

#define numLightDirectionValues 3
enum LightDirectionValueOffset {
    dirX = 0,
    dirY = 1,
    dirZ = 2
};

struct DirectionalLights : Lights {

    std::vector<double> directionToLights;

    DirectionalLights() {};
    void initializeVectors(int size) {
        directionToLights = std::vector<double>(size * numLightDirectionValues);
        Lights::initializeVectors(size);
    }
    
    void setLight(int i, Vector3<double> direction, Color lightColor, double intensity) {
        directionToLights[i * numLightDirectionValues + LightDirectionValueOffset::dirX] = direction.x();
        directionToLights[i * numLightDirectionValues + LightDirectionValueOffset::dirY] = direction.y();
        directionToLights[i * numLightDirectionValues + LightDirectionValueOffset::dirZ] = direction.z();
        setLightColor(i, lightColor, intensity);
    }

    Vector3<double> getDirectionToLight(int i) const { 
        return {
            directionToLights[i * numLightDirectionValues + LightDirectionValueOffset::dirX],
            directionToLights[i * numLightDirectionValues + LightDirectionValueOffset::dirY],
            directionToLights[i * numLightDirectionValues + LightDirectionValueOffset::dirZ]
        };
    }

    Color getLightColor(const int i) const {
        return Lights::getColor(i);
    }
};

#endif