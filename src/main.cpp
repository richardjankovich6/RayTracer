// #pragma once

#include <iostream>
#include <fstream>
#include<sstream>
// #include <cmath>

#include "globals.h"
#include "vector3.h"
#include "color.h"
// #include "point.h"
// #include "ray.h"
#include "sphere.h"

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

    Sphere purpleSphere;

    purpleSphere.setCenter({ .0, .0, .0 });
    purpleSphere.setRadius(0.4);
    purpleSphere.setKDiffiuse(0.4);
    purpleSphere.setKSpecular(0.7);
    purpleSphere.setKAmbient(0.1);
    purpleSphere.setDiffiuseColor({255, 0, 255});
    purpleSphere.setSpecularColor({255, 255, 255});
    purpleSphere.setKGloss(16.0);

    stringstream pic;

    // Render
    cout << clock() << "\n";

    pic << "P3\n" << width << " " << height << "\n255\n";

    Color c;

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            c = getColorDontUse(w, h);
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