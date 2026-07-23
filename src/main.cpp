// #pragma once

#include <iostream>
#include <fstream>
#include<sstream>
// #include <cmath>

// #include "globals.h"
#include "vector3.h"
#include "color.h"
#include "point.h"
#include "ray.h"
#include "sphere.h"

#include "camera.h"

using namespace std;

// void perspClip(int fovw, int fovh, double near, double far);


int maxDepth = 2;

// Vector3<> lookAt = { 0, 0, 0 };
// Vector3<> lookFrom = { 0, 0, 1 };
// Vector3<> lookUp = { 0, 1, 0 };
// Vector3<> dirToLight = { .0f, 1.f, .0f };
// Vector3<> lightColor = { 1., 1., 1. };
// Vector3<> ambientLight = { .0f, .0f, .0f };
// Vector3<> backgroundColor = { .2f, .2f, .2f };

int main() {

    // create camera
    Point lookAt = Point(0.0, 0.0, 0.0);
    Point lookFrom = Point(0.0, 0.0, 1.0);
    Vector3<double> lookUp = Vector3<double>(0.0, 1.0, 0.0);
    int width = 800;
    int height = 800;
    double fovWidth = 90.0;
    double fovHeight = 90.0;
    Camera camera = Camera(lookAt, lookFrom, lookUp, width, height, fovWidth, fovHeight);

    // create light
    Vector3<double> directionToLight = Vector3<double>(1.0, 1.0, 1.0);
    Color lightColor = Color(255, 255, 255);
    Color ambientLightColor = Color(25, 25, 25);
    Color backgroundColor = Color(10, 10, 10);
    Light light = Light(lightColor, directionToLight, ambientLightColor, backgroundColor);


    // create a purple sphere
    Sphere purpleSphere;

    purpleSphere.setCenter({ 0.45, .0, -0.15 });
    purpleSphere.setRadius(0.1);
    purpleSphere.setKDiffiuse(0.4);
    purpleSphere.setKSpecular(0.7);
    purpleSphere.setKAmbient(0.1);
    purpleSphere.setDiffiuseColor({255, 0, 255});
    purpleSphere.setSpecularColor({255, 255, 255});
    purpleSphere.setKGloss(16.0);
    purpleSphere.setLight(light);

    stringstream pic;

    // Render
    cout << clock() << "\n";

    pic << "P3\n" << width << " " << height << "\n255\n";

    // Vector3<double> lookDirection = unitVec(lookAt - lookFrom);
    Vector3<double> lookDirection = unitVec(lookFrom - lookAt);
    Color c;
    Ray r;
    Point p;

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            r = camera.makeRay(w, h);

            if (purpleSphere.intersetRay(p, r)) {
                c = purpleSphere.getColor(p, lookDirection);

            }
            else {
                c = light.getBackgroundColor();
            }
            
            // c = getColorDontUse(w, h);
            pic << c;
        }
        pic << '\n';
    }

    cout << "\n" << clock() << "\n";


    ofstream file("image.ppm");
    file << pic.str();
    
    return 0;
}

// void perspClip(int fovw, int fovh, double near, double far) {
//     double ar = static_cast<double>(width) / static_cast<double>(height);
//     double zoomw = 1 / tan(fovw / 2);
//     double zoomh = 1 / tan(fovh / 2);
//     double fn = near - far;
// }