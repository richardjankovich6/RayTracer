#pragma once

#include <vector>

#include "vector3.h"
#include "color.h"
#include "ray.h"
#include "camera.h"
#include "primitives/primitive.h"

#ifndef RAYTRACER_H
#define RAYTRACER_H


class Raytracer {

    Light light;
    Camera camera;
    int maxDepth = 0;

    void traceRaysTask(Color* &picArray, int heightStart, int heightEnd, int widthStart, int widthEnd) const;

public:

    std::vector<Primitive*> primitives;

    Raytracer() {};

    bool castShadowRay(const Ray ray, const int ignoredIndex) const;

    Color castRay(const Ray ray, const Vector3<double> lookDirection, int depth = 0, int ignoredIndex = -1) const;

    // void traceRays(std::stringstream& string) const;
    std::stringstream traceRays() const;

    void setCamera(Camera c) { camera = c; }
    void setLight(Light l) { light = l; }
    void setMaxDepth(int i) { maxDepth = i; }

};

#endif