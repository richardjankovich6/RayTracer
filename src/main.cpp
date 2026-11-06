// #pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>


#include "utils/jsonParsingHelper.h"

#include "raytracer.h"

int MAXDEPTH = 20;
int NUMTHREADS = 6;
int NUMSAMPLES = 5;

int main() {

    Raytracer raytracer;
    raytracer.setMaxDepth(MAXDEPTH);
    raytracer.setNumThreads(NUMTHREADS);
    raytracer.setNumSamples(NUMSAMPLES);

    raytracer.setCamera(JsonParsingHelper::parseCameraJson("../configs/camera.json"));
    JsonParsingHelper::parseLights(raytracer.lights, "../configs/lights.json");
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
