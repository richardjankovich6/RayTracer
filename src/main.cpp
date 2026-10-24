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

#include "primitives/primitive.h"
#include "primitives/sphere.h"
#include "primitives/triangle.h"


#include "raytracer.h"

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

    Point p1 = Point( 1.0, 1.0, 1.0);
    Point p2 = Point( -1.0, 1.0, 1.0);
    Point p3 = Point( 1.0, -1.0, 1.0);
    
    Triangle greyTriangle = Triangle(p1, p2, p3);


    greyTriangle.setKDiffiuse(0.6);
    greyTriangle.setKSpecular(0.7);
    greyTriangle.setKAmbient(0.1);
    greyTriangle.setReflectiveness(0.6);
    greyTriangle.setDiffiuseColor({0.3, 0.3, 0.3});
    greyTriangle.setSpecularColor({0.6, 0.6, 0.6});
    greyTriangle.setKGloss(2.0);
    greyTriangle.setLight(light);


    // vector<Primitive*> primitives = {&purpleSphere, &blueSphere, &redSphere, &greyTriangle};
    vector<Primitive*> primitives = {&purpleSphere, &blueSphere, &redSphere};


    Raytracer raytracer;
    raytracer.setCamera(camera);
    raytracer.setLight(light);
    raytracer.setMaxDepth(MAXDEPTH);
    raytracer.primitives = primitives;


    cout << clock() << "\n";

    // stringstream pic;
    // raytracer.traceRays(pic);

    stringstream pic = raytracer.traceRays();

    cout << "\n" << clock() << "\n";


    ofstream file("image.ppm");
    file << pic.str();
    
    return 0;
}
