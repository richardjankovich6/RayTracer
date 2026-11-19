#include "raytracer.h"
#include "primitives/primitives.h"
#include "vector3.h"
#include <iostream>
#include <sstream>
#include <thread>

bool Raytracer::castShadowRay(const Ray ray, const int ignoredIndex, const PrimitiveType ignoredType) const {
    for (int i = 0; i < spheres.numPrimitives; i++) {
        if (i == ignoredIndex && ignoredType == PrimitiveType::sphere) { continue; }
        Point p = ray.origin();
        if (spheres.intersetRay(i, p, ray)) {
            return true;
        }
    }
    for (int i = 0; i < triangles.numPrimitives; i++) {
        if (i == ignoredIndex && ignoredType == PrimitiveType::triangle) { continue; }
        Point p = ray.origin();
        if (triangles.intersetRay(i, p, ray)) {
            return true;
        }
    }
    return false;
}

Color Raytracer::castRay(const Ray ray, const Vector3<double> lookDirection, int depth, int ignoredIndex, PrimitiveType ignoredType) const {
    int nearestPrimitiveIndex = -1;
    PrimitiveType nearestType = PrimitiveType::none;
    
    double lowestDistance = std::numeric_limits<double>::max();
    Point currentPoint = Point();
    Point nearestPoint;

    for (int i = 0; i < spheres.numPrimitives; i++) {
        if (i == ignoredIndex && ignoredType == PrimitiveType::sphere) { continue; }
        if (!spheres.intersetRay(i, currentPoint, ray)) { continue; }
        
        double currentDistance = (currentPoint - ray.origin()).length();
        if (currentDistance >= lowestDistance) { continue; }

        nearestPrimitiveIndex = i;
        lowestDistance = currentDistance;
        nearestPoint = currentPoint;
        nearestType = PrimitiveType::sphere;
    }
    for (int i = 0; i < triangles.numPrimitives; i++) {
        if (i == ignoredIndex && ignoredType == PrimitiveType::triangle) { continue; }
        if (!triangles.intersetRay(i, currentPoint, ray)) { continue; }

        double currentDistance = (currentPoint - ray.origin()).length();
        if (currentDistance >= lowestDistance) { continue; }
        
        nearestPrimitiveIndex = i;
        lowestDistance = currentDistance;
        nearestPoint = currentPoint;
        nearestType = PrimitiveType::triangle;
    }

    if (nearestPrimitiveIndex == -1) {
        return Light::backgroundColor;
    }

    Color color = Color();
    // double lCont = 0;

    for (int i = 0; i < directionalLights.numLights; i++) {
        Vector3<double> dirToLight = directionalLights.getDirectionToLight(i);
        bool bInShadow = castShadowRay(Ray(nearestPoint, dirToLight), nearestPrimitiveIndex, nearestType);

        if (bInShadow) { continue; }

        Color lightColor = directionalLights.getLightColor(i);
        color += nearestType == PrimitiveType::sphere ? 
            spheres.getColor(nearestPrimitiveIndex, nearestPoint, lookDirection, dirToLight, lightColor, bInShadow) :
            triangles.getColor(nearestPrimitiveIndex, nearestPoint, lookDirection, dirToLight, lightColor, bInShadow);

        // lCont += 1;
    }

    for (int i = 0; i < pointLights.numLights; i++) {
        Vector3<double> dirToLight = pointLights.getDirectionToLight(i, nearestPoint);
        bool bInShadow = castShadowRay(Ray(nearestPoint, dirToLight), nearestPrimitiveIndex, nearestType);

        if (bInShadow) { continue; }

        Color lightColor = pointLights.getLightColor(i, nearestPoint);
        color += nearestType == PrimitiveType::sphere ? 
            spheres.getColor(nearestPrimitiveIndex, nearestPoint, lookDirection, dirToLight, lightColor, bInShadow) :
            triangles.getColor(nearestPrimitiveIndex, nearestPoint, lookDirection, dirToLight, lightColor, bInShadow);

        // lCont += 1;
    }


    double reflectiveness = 
        nearestType == PrimitiveType::sphere ?
            spheres.materialValues[nearestPrimitiveIndex * numMaterialValues + MaterialOffset::reflectiveness] :
            triangles.materialValues[nearestPrimitiveIndex * numMaterialValues + MaterialOffset::reflectiveness];

    if (depth < maxDepth && reflectiveness > 0) {

        Ray reflectRay = nearestType == PrimitiveType::sphere ?
            Ray(nearestPoint, spheres.getNormal(nearestPrimitiveIndex, nearestPoint)) :
            Ray(nearestPoint, triangles.getNormal(nearestPrimitiveIndex));

        Color recursiveColor = castRay(reflectRay, lookDirection, depth + 1, nearestPrimitiveIndex, nearestType);

        color = recursiveColor + color * (1 - reflectiveness);
    }

    return color;
}

void Raytracer::traceRaysTask(Color* picArray, int heightStart, int heightEnd, int widthStart, int widthEnd) const {
    for (int h = heightStart; h < heightEnd; h++) {
        for (int w = widthStart; w < widthEnd; w++) {
            Color c = Color(0.0,0.0,0.0);
            for (int i = 0; i < numSamples; i++) {
                Ray r = camera.makeRandomRay(w, h);
                c += castRay(r, lookDirection, 0);
            }
            picArray[w + h * camera.width] = c / numSamples;
        }
    }
}

void Raytracer::traceRaysTaskMutex(Color* picArray, int heightStart, int heightEnd) {
    int curW = 0;
    double inversSamples = 1.0 / static_cast<double>(numSamples);
    while (currentWidth < camera.width) {
        editCurrentWidth.lock();
        curW = currentWidth;
        currentWidth += threadIncrimentSize;
        editCurrentWidth.unlock();
        for (int w = curW; w < curW + threadIncrimentSize; w++) {
            for (int h = 0; h < camera.height; h++) {
                Color c = Color(0.0,0.0,0.0);
                for (int i = 0; i < numSamples; i++) {
                    c += castRay(camera.makeRandomRay(w, h), lookDirection, 0);
                }
                picArray[w + h * camera.width] = c * inversSamples;

                // Color c = castRay(camera.makeRay(w, h), lookDirection, 0);
                // picArray[w + h * camera.width] = c;
            }
        }
        
    }
}

std::stringstream Raytracer::traceRays() {
    lookDirection = unitVec(camera.lookFrom - camera.lookAt);
    Color* picArray = new Color[camera.width*camera.height];
    // std::vector<Color> picArray = std::vector<Color>(camera.width*camera.height);

    // traceRaysTask(picArray, 0, camera.height, 641, camera.width-300);
    // traceRaysTask(picArray, 300, camera.height, 300, camera.width-300);

    std::jthread* threads = new std::jthread[numThreads];
    int x = camera.width / numThreads;
    for (int i = 0; i < numThreads; i++) {
        threads[i] = std::jthread(&Raytracer::traceRaysTaskMutex, this, picArray, 0, camera.height);
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