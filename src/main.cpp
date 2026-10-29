// #pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
// #include <vector>

#include "vector3.h"
#include "color.h"
#include "point.h"
#include "camera.h"
#include "lights/directionalLight.h"

// #include "primitives/primitive.h"
#include "utils/jsonParsingHelper.h"

#include "raytracer.h"

int MAXDEPTH = 20;
int NUMTHREADS = 6;

Camera declareCamera() {
    // declare constants

    Point lookAt = Point(0.0, 0.0, 0.0);
    Point lookFrom = Point(0.0, 0.0, 3.0);
    Vector3<double> lookUp = Vector3<double>(0.0, 1.0, 0.0);
    constexpr int width = 1920;
    constexpr int height = 1080;
    constexpr double fov = 90.0;
    return Camera(lookAt, lookFrom, lookUp, width, height, fov);
}

DirectionalLight declareLight() {
    // create light
    Vector3<double> directionToLight = Vector3<double>(0.4, 1.0, 0.4);
    // Vector3<double> directionToLight = Vector3<double>(0.6, 0.8, 1.0);
    Color lightColor = Color(1.0, 1.0, 1.0);
    Color ambientLightColor = Color(0.3, 0.3, 0.3);
    Color backgroundColor = Color(0.04, 0.04, 0.04);
    return DirectionalLight(lightColor, directionToLight, ambientLightColor, backgroundColor);
}


int main() {

    DirectionalLight light = declareLight();

    Camera camera = JsonParsingHelper::parseCameraJson("../configs/camera.json");

    Raytracer raytracer;
    raytracer.setCamera(camera);
    raytracer.setLight(light);
    raytracer.setMaxDepth(MAXDEPTH);
    raytracer.setNumThreads(NUMTHREADS);
    JsonParsingHelper::parsePrimitives(raytracer.primitives, "../configs/scenes/boxScene1.json", &light);

    auto start = std::chrono::high_resolution_clock::now();

    std::stringstream pic = raytracer.traceRays();
    std::ofstream file("image.ppm");
    file << pic.str();

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "total duration in milliseconds: " << duration << std::endl;

    // std::cout << sizeof(Triangle) << std::endl;
    // std::cout << sizeof(Sphere) << std::endl;
    
    return 0;
}
