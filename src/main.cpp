// #pragma once

#include <iostream>
#include <fstream>
#include<sstream>
#include <cmath>

#include "globals.h"

#include "vector3.h"
#include "color.h"
#include "point.h"
#include "ray.h"



// int main() {
//     // Engine::Vector3 vec = Engine::Vector3();
//     // std::cout << vec << std::endl;

//     Vector3 vec = Vector3<int>();

//     std::cout << vec << std::endl;

//     return 0;
// }

//#include "stdio.h"


using namespace std;

void perspClip(int fovw, int fovh, double near, double far);

void castRay();

int maxDepth = 2;

Vector3<> lookAt = { 0, 0, 0 };
Vector3<> lookFrom = { 0, 0, 1 };
Vector3<> lookUp = { 0, 1, 0 };
Vector3<> dirToLight = { .0f, 1.f, .0f };
Vector3<> lightColor = { 1., 1., 1. };
Vector3<> ambientLight = { .0f, .0f, .0f };
Vector3<> backgroundColor = { .2f, .2f, .2f };

struct Sphere {
    Vector3<> center;
    float radius;
    float kd;
    float ks;
    float ka;
    Vector3<> od;
    Vector3<> os;
    float kgls;
};
Sphere purpleSphere;

int main() {

    purpleSphere.center = { .0, .0, .0 };
    purpleSphere.radius = 0.4f;
    purpleSphere.kd = 0.7f;
    purpleSphere.ks = 0.2f;
    purpleSphere.ka = 0.1f;
    purpleSphere.od = { 1., .0, 1. };
    purpleSphere.os = { 1., 1., 1. };
    purpleSphere.kgls = 16.0f;

    stringstream pic;

    // Render
    cout << clock() << "\n";

    pic << "P3\n" << width << " " << height << "\n255\n";

    color c;

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

void perspClip(int fovw, int fovh, double near, double far) {
    double ar = static_cast<double>(width) / static_cast<double>(height);
    double zoomw = 1 / tan(fovw / 2);
    double zoomh = 1 / tan(fovh / 2);
    double fn = near - far;
}