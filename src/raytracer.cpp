#include "raytracer.h"
#include "primitives/primitives.h"
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
    int totalPrimitives = triangles.numPrimitives + spheres.numPrimitives;
    int nearestPrimitiveIndex = -1;
    PrimitiveType nearestType = PrimitiveType::none;
    
    double lowestDistance = std::numeric_limits<double>::max();
    Point currentPoint = Point();
    Point nearestPoint;

    for (int i = 0; i < spheres.numPrimitives; i++) {
        if (i == ignoredIndex && ignoredType == PrimitiveType::sphere) { continue; }
        if (spheres.intersetRay(i, currentPoint, ray)) {
            double currentDistance = (currentPoint - ray.origin()).length();
            if (currentDistance < lowestDistance) { 
                nearestPrimitiveIndex = i;
                lowestDistance = currentDistance;
                nearestPoint = currentPoint;
                nearestType = PrimitiveType::sphere;
            }
        }
    }
    for (int i = 0; i < triangles.numPrimitives; i++) {
        if (i == ignoredIndex && ignoredType == PrimitiveType::triangle) { continue; }
        if (triangles.intersetRay(i, currentPoint, ray)) {
            double currentDistance = (currentPoint - ray.origin()).length();
            if (currentDistance < lowestDistance) { 
                nearestPrimitiveIndex = i;
                lowestDistance = currentDistance;
                nearestPoint = currentPoint;
                nearestType = PrimitiveType::triangle;
            }
        }
    }

    if (nearestPrimitiveIndex == -1) {
        return Light::backgroundColor;
    }

    int totalLights = lights.size();

    Color color = Color();
    // Vector3<int> lightsContributed = Vector3<int>(0, 0, 0);
    // double lCont = 0;

    for (int i = 0; i < totalLights; i++) {
        bool bInShadow = castShadowRay(Ray(nearestPoint, lights[i]->getDirectionToLight()), nearestPrimitiveIndex, nearestType);

        if (bInShadow) { continue; }

        Color temp;
        if (nearestType == PrimitiveType::sphere) {
            temp = spheres.getColor(nearestPrimitiveIndex, nearestPoint, lookDirection, lights[i], bInShadow);
        }
        else {
            temp = triangles.getColor(nearestPrimitiveIndex, lookDirection, lights[i], bInShadow);
        }
        // Color temp = primitives[nearestPrimitiveIndex]->getColor(nearestPoint, lookDirection, lights[i], bInShadow);
        // Color temp = primitives[nearestPrimitiveIndex]->getColor(nearestPoint, lookDirection, lights[i], bInShadow);
        // for (int o = 0; o < 3; o++) {
        //     lightsContributed[0] += temp[0] > 0 ? 1 : 0;
        // }
        color += temp;
        // lCont += 1;
    }

    // for (int o = 0; o < 3; o++) {
    //     color[o] = lightsContributed[o] != 0 ? color[o] / lightsContributed[o] : color[o];
    // }

    // color = lCont != 0 ? color / lCont : color;

    // color[0] /= lightsContributed[0];

    // color /= lightsContributed;

    // color /= totalLights;

    double reflectiveness = 
        nearestType == PrimitiveType::sphere ?
            spheres.materialValues[nearestPrimitiveIndex * numMaterialValues + MaterialOffset::reflectiveness] :
            triangles.materialValues[nearestPrimitiveIndex * numMaterialValues + MaterialOffset::reflectiveness];

    if (depth < maxDepth && reflectiveness > 0) {
    // if (depth < MAXDEPTH) {

        Ray reflectRay;
        if (nearestType == PrimitiveType::sphere) {
            reflectRay = Ray(nearestPoint, spheres.getNormal(nearestPrimitiveIndex, nearestPoint));
        }
        else {
            reflectRay = Ray(nearestPoint, triangles.getNormal(nearestPrimitiveIndex));
        }
        //  = Ray(nearestPoint, primitives[nearestPrimitiveIndex]->getNormal(nearestPoint));

        Color recursiveColor = castRay(reflectRay, lookDirection, depth + 1, nearestPrimitiveIndex);


        color = recursiveColor + color * (1 - reflectiveness);

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
        curW = currentWidth;
        currentWidth += threadIncrimentSize;
        editCurrentWidth.unlock();
        for (int w = curW; w < curW + threadIncrimentSize; w++) {
            for (int h = 0; h < camera.height; h++) {
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
}

std::stringstream Raytracer::traceRays() {
    lookDirection = unitVec(camera.lookFrom - camera.lookAt);
    Color* picArray = new Color[camera.width*camera.height];
    // std::vector<Color> picArray = std::vector<Color>(camera.width*camera.height);

    // traceRaysTask(picArray, 200, camera.height, 200, camera.width-300);
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