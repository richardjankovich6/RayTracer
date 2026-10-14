// #pragma once

#include <iostream>
#include <fstream>
#include<sstream>
#include <vector>

#include "vector3.h"
#include "color.h"
#include "point.h"
#include "ray.h"

#include "camera.h"

#include "primitive.h"
#include "sphere.h"
#include "triangle.h"

using namespace std;

int MAXDEPTH = 20;

Camera declareCamera() {
    // declare constants

    Point lookAt = Point(0.0, 0.0, 0.0);
    Point lookFrom = Point(0.0, 0.0, 1.0);
    Vector3<double> lookUp = Vector3<double>(0.0, 1.0, 0.0);
    constexpr int width = 800;
    constexpr int height = 800;
    constexpr double fov = 90.0;
    return Camera(lookAt, lookFrom, lookUp, width, height, fov);
}

Light declareLight() {
    // create light
    Vector3<double> directionToLight = Vector3<double>(1.0, 1.0, 1.0);
    Color lightColor = Color(1.0, 1.0, 1.0);
    Color ambientLightColor = Color(0.3, 0.3, 0.3);
    Color backgroundColor = Color(0.04, 0.04, 0.04);
    return Light(lightColor, directionToLight, ambientLightColor, backgroundColor);
}

bool castShadowRay(Ray ray, const vector<Primitive*> primitives, int ignoredIndex) {
    for (int i = 0; i < primitives.size(); i++) {
        if (i == ignoredIndex) { continue; }
        Point p = ray.origin();
        if (primitives[i]->intersetRay(p, ray)) {
            return true;
        }
    }
    // int nearestPrimitiveIndex = -1;
    return false;
}

// Color castRay(Ray, Vector3<double> lookDirection, const vector<Sphere> spheres, const vector<Triangle> triangles, int depth = 0) {
Color castRay(const Ray ray, const Vector3<double> lookDirection, const vector<Primitive*> primitives, Light light, int depth = 0, int ignoredIndex = -1) {

    vector<Point> points = {};
    // int totalPrimitives = spheres.size() + triangles.size();
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

    bool inShadow = castShadowRay(Ray(nearestPoint, light.directionToLight), primitives, nearestPrimitiveIndex);

    Color color = primitives[nearestPrimitiveIndex]->getColor(nearestPoint, lookDirection, inShadow);

    if (depth < MAXDEPTH && primitives[nearestPrimitiveIndex]->getReflectiveness() > 0) {
    // if (depth < MAXDEPTH) {

        Ray reflectRay = Ray(nearestPoint, primitives[nearestPrimitiveIndex]->getNormal(nearestPoint));

        Color recursiveColor = castRay(reflectRay, lookDirection, primitives, light, depth + 1, nearestPrimitiveIndex);


        color = recursiveColor + color * (1 - primitives[nearestPrimitiveIndex]->getReflectiveness());

    }

    return color;

}

int main() {

    Camera camera = declareCamera();
    Light light = declareLight();

    
    // create a purple sphere
    Sphere purpleSphere;
    purpleSphere.setCenter(Vector3( 0.0, 0.0, 0.0));
    purpleSphere.setRadius(0.3);
    purpleSphere.setKDiffiuse(0.6);
    purpleSphere.setKSpecular(0.2);
    purpleSphere.setKAmbient(0.1);
    purpleSphere.setReflectiveness(0.0);
    purpleSphere.setDiffiuseColor({1.0, 0, 1.0});
    purpleSphere.setSpecularColor({0.6, 0.6, 0.6});
    purpleSphere.setKGloss(2.0);
    purpleSphere.setLight(light);

    Sphere blueSphere;
    blueSphere.setCenter(Vector3( -1.2, 0.2, -0.4));
    blueSphere.setRadius(0.4);
    blueSphere.setKDiffiuse(0.9);
    blueSphere.setKSpecular(0.2);
    blueSphere.setKAmbient(0.1);
    blueSphere.setReflectiveness(0.0);
    blueSphere.setDiffiuseColor({0.0, 0, 1.0});
    blueSphere.setSpecularColor({0.6, 0.6, 0.6});
    blueSphere.setKGloss(2.0);
    blueSphere.setLight(light);

    Sphere redSphere;
    redSphere.setCenter(Vector3( -0.8, 0.7, -0.8));
    redSphere.setRadius(0.4);
    redSphere.setKDiffiuse(0.6);
    redSphere.setKSpecular(0.7);
    redSphere.setKAmbient(0.1);
    redSphere.setReflectiveness(0.6);
    redSphere.setDiffiuseColor({1.0, 0.1, 0.1});
    redSphere.setSpecularColor({0.6, 0.6, 0.6});
    redSphere.setKGloss(2.0);
    redSphere.setLight(light);

    vector<Primitive*> primitives = {&purpleSphere, &blueSphere, &redSphere};

    stringstream pic;

    // Render
    pic << "P3\n" << camera.width << " " << camera.height << "\n255\n";

    Vector3<double> lookDirection = unitVec(camera.lookFrom - camera.lookAt);
    Color c;
    Ray r;
    Point p;

    cout << clock() << "\n";
    for (int h = 0; h < camera.height; h++) {
        for (int w = 0; w < camera.width; w++) {
            r = camera.makeRay(w, h);

            c = castRay(r, lookDirection, primitives, light, 0);

            // if (blueSphere.intersetRay(p, r)) {
            //     // c = purpleSphere.getColor(p, lookDirection);
            //     c = blueSphere.getColor(p, lookDirection);

            // }
            // else {
            //     c = light.getBackgroundColor();
            // }
            pic << c;
        }
        pic << '\n';
    }
    cout << "\n" << clock() << "\n";


    ofstream file("image.ppm");
    file << pic.str();
    
    return 0;
}
