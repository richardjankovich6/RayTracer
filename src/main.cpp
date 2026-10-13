// #pragma once

#include <iostream>
#include <fstream>
#include<sstream>

#include "vector3.h"
#include "color.h"
#include "point.h"
#include "ray.h"
#include "sphere.h"
#include "camera.h"

using namespace std;


int maxDepth = 2;

int main() {

    // create camera
    Point lookAt = Point(0.0, 0.0, 0.0);
    Point lookFrom = Point(0.0, 0.0, 1.0);
    Vector3<double> lookUp = Vector3<double>(0.0, 1.0, 0.0);
    int width = 800;
    int height = 800;
    double fovWidth = 100.0;
    double fovHeight = 100.0;
    Camera camera = Camera(lookAt, lookFrom, lookUp, width, height, fovWidth, fovHeight);

    // create light
    Vector3<double> directionToLight = Vector3<double>(1.0, 1.0, 1.0);
    Color lightColor = Color(1.0, 1.0, 1.0);
    Color ambientLightColor = Color(0.3, 0.3, 0.3);
    Color backgroundColor = Color(0.04, 0.04, 0.04);
    Light light = Light(lightColor, directionToLight, ambientLightColor, backgroundColor);


    // create a purple sphere
    Sphere purpleSphere;

    // purpleSphere.setCenter(Vector3( 2.0, 2.0, 0.0));
    purpleSphere.setCenter(Vector3( 0.0, 0.0, 0.0));
    purpleSphere.setRadius(0.3);
    purpleSphere.setKDiffiuse(0.6);
    purpleSphere.setKSpecular(0.2);
    purpleSphere.setKAmbient(0.1);
    purpleSphere.setDiffiuseColor({1.0, 0, 1.0});
    purpleSphere.setSpecularColor({0.6, 0.6, 0.6});
    purpleSphere.setKGloss(2.0);
    purpleSphere.setLight(light);

    stringstream pic;

    // Render
    cout << clock() << "\n";

    pic << "P3\n" << width << " " << height << "\n255\n";

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
            pic << c;
        }
        pic << '\n';
    }

    cout << "\n" << clock() << "\n";


    ofstream file("image.ppm");
    file << pic.str();
    
    return 0;
}
