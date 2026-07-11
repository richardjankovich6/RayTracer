// #pragma once

#include <iostream>
// #include "utils.h"
#include "vector3.h"



// int main() {
//     // Engine::Vector3 vec = Engine::Vector3();
//     // std::cout << vec << std::endl;

//     Vector3 vec = Vector3<int>();

//     std::cout << vec << std::endl;

//     return 0;
// }

//#include "stdio.h"
#include <iostream>
#include <fstream>
#include<sstream>

#include <cmath>


using namespace std;

using color = Vector3<int>;
std::ostream& operator<<(std::ostream& out, const color& v) {
    return out << v.vec[0] << ", " << v.vec[1] << ", " << v.vec[2] << "  ";
}


color getColor(int w, int h);
void perspClip(int fovw, int fovh, double near, double far);



void castRay();

int maxDepth = 2;

constexpr int width = 1920;
constexpr int height = 1080;
constexpr int fov = 90;
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


class ray {
public:
    ray() {}
    ray(const point3& origin, const Vector3<>& direction) : orig(origin), dir(direction) {}
    const point3& origin() const { return orig; }
    const Vector3<>& direction() const { return dir; }
    point3 at(double t) const { return orig + t * dir; }
private:
    point3 orig;
    Vector3<> dir;
};


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
            c = getColor(w, h);
            pic << c;
        }
        pic << '\n';
    }

    cout << "\n" << clock() << "\n";


    ofstream file("image.ppm");
    file << pic.str();
    return 0;
}
color getColor(int w, int h) {

    float r = static_cast<float>(h) / (height - 1);
    float g = static_cast<float>(w) / (width - 1);
    double b = 0.0;

    int ir = static_cast<int>(255.999 * r);
    int ig = static_cast<int>(255.999 * g);
    int ib = static_cast<int>(255.999 * b);

    return Vector3<int>(ir, ig, ib);
}

void castRay() {
    return;
}

void perspClip(int fovw, int fovh, double near, double far) {
    double ar = static_cast<double>(width) / static_cast<double>(height);
    double zoomw = 1 / tan(fovw / 2);
    double zoomh = 1 / tan(fovh / 2);
    double fn = near - far;
}