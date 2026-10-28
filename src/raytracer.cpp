#include "raytracer.h"
#include<sstream>


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
        return light.getBackgroundColor();
    }

    bool inShadow = castShadowRay(Ray(nearestPoint, light.directionToLight), nearestPrimitiveIndex);

    Color color = primitives[nearestPrimitiveIndex]->getColor(nearestPoint, lookDirection, inShadow);

    if (depth < maxDepth && primitives[nearestPrimitiveIndex]->getReflectiveness() > 0) {
    // if (depth < MAXDEPTH) {

        Ray reflectRay = Ray(nearestPoint, primitives[nearestPrimitiveIndex]->getNormal(nearestPoint));

        Color recursiveColor = castRay(reflectRay, lookDirection, depth + 1, nearestPrimitiveIndex);


        color = recursiveColor + color * (1 - primitives[nearestPrimitiveIndex]->getReflectiveness());

    }

    return color;
}

void Raytracer::traceRaysTask(Color* &picArray, int heightStart, int heightEnd, int widthStart, int widthEnd) const {
    Vector3<double> lookDirection = unitVec(camera.lookFrom - camera.lookAt);
    Color c;
    Ray r;
    for (int h = heightStart; h < heightEnd; h++) {
        for (int w = widthStart; w < widthEnd; w++) {
            r = camera.makeRay(w, h);
            c = castRay(r, lookDirection, 0);
            picArray[w + h * camera.width] = c;
        }
    }

}

std::stringstream Raytracer::traceRays() const {
    Color* picArray = new Color[camera.width*camera.height];

    traceRaysTask(picArray, 0, 0, camera.height, camera.width);

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