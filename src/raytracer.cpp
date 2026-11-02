#include "raytracer.h"
#include <iostream>
#include<sstream>
#include <thread>

bool Raytracer::castShadowRay(const Ray ray, const int ignoredIndex) const {
    for (int i = 0; i < primitives.size(); i++) {
        if (i == ignoredIndex) { continue; }
        Point p = ray.origin();
        if (primitives[i]->intersetRay(p, ray)) {
            return true;
        }
    }
    return false;
}

Color Raytracer::castRay(const Ray ray, const Vector3<double> lookDirection, int depth, int ignoredIndex) const {
    int totalPrimitives = primitives.size();
    int nearestPrimitiveIndex = -1;
    double lowestDistance = std::numeric_limits<double>::max();
    Point currentPoint = Point();
    Point nearestPoint;

    for (int i = 0; i < totalPrimitives; i++) {
        if (i == ignoredIndex) { continue; }
        if (primitives[i]->intersetRay(currentPoint, ray)) {
            double currentDistance = (currentPoint - ray.origin()).length();
            if (currentDistance < lowestDistance) { 
                nearestPrimitiveIndex = i;
                lowestDistance = currentDistance;
                nearestPoint = currentPoint;
            }
        }
    }

    if (nearestPrimitiveIndex == -1) {
        return light->getBackgroundColor();
    }

    bool inShadow = castShadowRay(Ray(nearestPoint, light->getDirectionToLight()), nearestPrimitiveIndex);

    Color color = primitives[nearestPrimitiveIndex]->getColor(nearestPoint, lookDirection, inShadow);

    if (depth < maxDepth && primitives[nearestPrimitiveIndex]->getReflectiveness() > 0) {
    // if (depth < MAXDEPTH) {

        Ray reflectRay = Ray(nearestPoint, primitives[nearestPrimitiveIndex]->getNormal(nearestPoint));

        Color recursiveColor = castRay(reflectRay, lookDirection, depth + 1, nearestPrimitiveIndex);


        color = recursiveColor + color * (1 - primitives[nearestPrimitiveIndex]->getReflectiveness());

    }

    return color;
}

void Raytracer::traceRaysTask(Color* picArray, int heightStart, int heightEnd, int widthStart, int widthEnd) const {
    Color c;
    Ray r;

    for (int h = heightStart; h < heightEnd; h++) {
        for (int w = widthStart; w < widthEnd; w++) {
            c = Color(0.0,0.0,0.0);
            for (int i = 0; i < numSamples; i++) {
                // r = camera.makeRay(w, h);
                r = camera.makeRandomRay(w, h);
                c += castRay(r, lookDirection, 0);
            }
            picArray[w + h * camera.width] = c / numSamples;
        }
    }
}

void Raytracer::traceRaysTaskMutex(Color* picArray, int heightStart, int heightEnd) {
    Color c;
    Ray r;

    int curW = 0;
    while (currentWidth < camera.width) {
        editCurrentWidth.lock();
        curW = currentWidth++;
        editCurrentWidth.unlock();
        for (int h = 0; h < camera.height; h++) {
             c = Color(0.0,0.0,0.0);
            for (int i = 0; i < numSamples; i++) {
                // r = camera.makeRay(w, h);
                r = camera.makeRandomRay(curW, h);
                c += castRay(r, lookDirection, 0);
            }
            picArray[curW + h * camera.width] = c / numSamples;
        }
    }
}

std::stringstream Raytracer::traceRays() {
    lookDirection = unitVec(camera.lookFrom - camera.lookAt);
    Color* picArray = new Color[camera.width*camera.height];

    // traceRaysTask(picArray, 300, camera.height, 300, camera.width);

    std::jthread* threads = new std::jthread[numThreads];
    int x = camera.width / numThreads;
    for (int i = 0; i < numThreads; i++) {
        // threads[i] = std::jthread(&Raytracer::traceRaysTask, this, picArray, 0, camera.height, i * x, (i + 1) * x);
        threads[i] = std::jthread(&Raytracer::traceRaysTaskMutex, this, picArray, 0, camera.height);
        std::cout << i << " " << i * x << " " << (i + 1) * x << std::endl;
    }
    for (int i = 0; i < numThreads; i++) {
        threads[i].join();
    }
    delete[] threads;

    std::stringstream string;
    string << "P3\n" << camera.width << " " << camera.height << "\n255\n";
    for (int h = 0; h < camera.height; h++) {
        for (int w = 0; w < camera.width; w++) {
            string << picArray[w + h * camera.width];
        }
        string << '\n';
    }

    
    delete[] picArray;

    return string;
}