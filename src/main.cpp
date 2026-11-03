// #pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

#include "vector3.h"
#include "color.h"
#include "camera.h"
#include "lights/directionalLight.h"
#include "lights/pointLight.h"
#include "lights/light.h"

#include "utils/jsonParsingHelper.h"

#include "raytracer.h"

int MAXDEPTH = 20;
int NUMTHREADS = 6;
int NUMSAMPLES = 5;

DirectionalLight declareLight() {
    // create light
    Vector3<double> directionToLight = Vector3<double>(0.4, 1.0, 0.4);
    Color lightColor = Color(1.0, 1.0, 1.0);
    return DirectionalLight(lightColor, directionToLight);
}

int main() {

    Light::ambientLight = Color(0.3, 0.3, 0.3);
    Light::backgroundColor = Color(0.04, 0.04, 0.04);

    DirectionalLight dlight = declareLight();


    PointLight plight = PointLight(Color(0.5, 0.8, 0.2), Point(-3.0,-3.0,1.0), 100.0);
    // PointLight plight = PointLight(Color(1.0, 1.0, 1.0), Point(0.0,2.0,1.0), 100.0);

    Light* light = &plight;

    Camera camera = JsonParsingHelper::parseCameraJson("../configs/camera.json");

    Raytracer raytracer;
    raytracer.setCamera(camera);
    raytracer.lights.push_back(&plight);
    raytracer.lights.push_back(&dlight);
    // raytracer.setLight(light);
    raytracer.setMaxDepth(MAXDEPTH);
    raytracer.setNumThreads(NUMTHREADS);
    raytracer.setNumSamples(NUMSAMPLES);
    JsonParsingHelper::parsePrimitives(raytracer.primitives, "../configs/scenes/boxScene1.json");

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
