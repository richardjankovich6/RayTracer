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

int maxDepth = 2;

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

// Color castRay(Ray, Vector3<double> lookDirection, const vector<Sphere> spheres, const vector<Triangle> triangles, int depth = 0) {
Color castRay(const Ray ray, const Vector3<double> lookDirection, const vector<Primitive*> primitives, int depth = 0) {

    vector<Point> points = {};
    // int totalPrimitives = spheres.size() + triangles.size();
    int totalPrimitives = primitives.size();
    int nearestPrimitiveIndex = -1;
    double lowestDistance = std::numeric_limits<double>::max();
    Point currentPoint = Point();
    Point nearestPoint;

    for (int i = 0; i < totalPrimitives; i++) {
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
        return primitives[0]->getBackgroundColor();
    }

    return primitives[nearestPrimitiveIndex]->getColor(nearestPoint, lookDirection);

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
    purpleSphere.setDiffiuseColor({1.0, 0, 1.0});
    purpleSphere.setSpecularColor({0.6, 0.6, 0.6});
    purpleSphere.setKGloss(2.0);
    purpleSphere.setLight(light);

    Sphere blueSphere;
    blueSphere.setCenter(Vector3( -1.2, 0.2, -0.8));
    blueSphere.setRadius(0.4);
    blueSphere.setKDiffiuse(0.9);
    blueSphere.setKSpecular(0.2);
    blueSphere.setKAmbient(0.1);
    blueSphere.setDiffiuseColor({0.0, 0, 1.0});
    blueSphere.setSpecularColor({0.6, 0.6, 0.6});
    blueSphere.setKGloss(2.0);
    blueSphere.setLight(light);

    // Sphere spheres[] = {purpleSphere, blueSphere};

    vector<Primitive*> primitives = {&purpleSphere, &blueSphere};
    // vector<Primitive*> primitives;
    // primitives.push_back(&purpleSphere);
    //  {purpleSphere, blueSphere};
    
    // vector<Sphere> spheres = {purpleSphere, blueSphere};
    // vector<Triangle> triangles = {};

    stringstream pic;

    // Render
    cout << clock() << "\n";

    pic << "P3\n" << camera.width << " " << camera.height << "\n255\n";

    Vector3<double> lookDirection = unitVec(camera.lookFrom - camera.lookAt);
    Color c;
    Ray r;
    Point p;

    for (int h = 0; h < camera.height; h++) {
        for (int w = 0; w < camera.width; w++) {
            r = camera.makeRay(w, h);

            c = castRay(r, lookDirection, primitives, 0);

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
