#pragma once

#include <vector>
#include <mutex>

#include "vector3.h"
#include "color.h"
#include "ray.h"
#include "camera.h"
// #include "primitives/primitive.h"
#include "primitives/primitives.h"

class Light;

#ifndef RAYTRACER_H
#define RAYTRACER_H

class Raytracer {

    Camera camera;
    int maxDepth = 1;
    int numThreads = 1;
    int numSamples = 1;
    int threadIncrimentSize = 10;

    std::mutex editCurrentWidth;
    int currentWidth = 0;

    Vector3<double> lookDirection;

    void traceRaysTask(Color* picArray, int heightStart, int heightEnd, int widthStart, int widthEnd) const;
    void traceRaysTaskMutex(Color* picArray, int heightStart, int heightEnd);

    bool castShadowRay(const Ray ray, const int ignoredIndex) const;

    Color castRay(const Ray ray, const Vector3<double> lookDirection, int depth = 0, int ignoredIndex = -1) const;

public:

    // std::vector<Primitives*> primitives;

    Triangles Triangles;
    Spheres spheres;
    
    std::vector<Light*> lights;

    Raytracer() {};

    std::stringstream traceRays();

    void setCamera(Camera c) { camera = c; }
    void setMaxDepth(int i) { maxDepth = i; }
    void setNumThreads (int i) { numThreads = i > 0 ? i : 1; }
    void setNumSamples (int i) { numSamples = i > 0 ? i : 1; }

};

#endif