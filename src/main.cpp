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

Ray makeRay(Camera camera, int w, int h) {

    double x = -camera.widthExtent + camera.halfPixelWidth * (static_cast<double>(w) + 0.5) * 2;
    double y = camera.heightExtent - camera.halfPixelHeight * (static_cast<double>(h) + 0.5) * 2;

    Vector3<double> direction = Vector3<double>(x, y, camera.lookAt.z()) - camera.lookFrom;

    return Ray(camera.lookFrom, unitVec(direction));
}


Camera declareCamera() {
    // declare constants

    Point lookAt = Point(0.0, 0.0, 0.0);
    Point lookFrom = Point(0.0, 0.0, 1.0);
    Vector3<double> lookUp = Vector3<double>(0.0, 1.0, 0.0);
    constexpr int width = 800;
    constexpr int height = 800;
    constexpr double fov = 90.0;

    // calculate camera values

    double magnitude = (lookAt - lookFrom).length();

    double aspectRatio = static_cast<double>(width) / static_cast<double>(height);
    double tangentHeight = tan(fov / aspectRatio / 2 * M_PI / 180.0);
    double tangentWidth = tan(fov / 2 * M_PI / 180.0);

    double widthExtent = abs(tangentWidth * magnitude);
    double heightExtent = abs(tangentHeight * magnitude);

    double halfPixelWidth = widthExtent / width;
    double halfPixelHeight = heightExtent / height;

    // create camera
    Camera camera;
    camera.lookAt = lookAt;
    camera.lookFrom = lookFrom;
    camera.lookUp = lookUp;
    camera.width = width;
    camera.height = height;
    camera.fov = fov;
    camera.widthExtent = widthExtent;
    camera.heightExtent = heightExtent;
    camera.halfPixelWidth = halfPixelWidth;
    camera.halfPixelHeight = halfPixelHeight;

    return camera;
}

Light declareLight() {
    // create light
    Vector3<double> directionToLight = Vector3<double>(1.0, 1.0, 1.0);
    Color lightColor = Color(1.0, 1.0, 1.0);
    Color ambientLightColor = Color(0.3, 0.3, 0.3);
    Color backgroundColor = Color(0.04, 0.04, 0.04);
    Light light = Light(lightColor, directionToLight, ambientLightColor, backgroundColor);
    return light;
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

    Sphere spheres[] = {purpleSphere, blueSphere};

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
            r = makeRay(camera, w, h);

            if (purpleSphere.intersetRay(p, r)) {
                c = purpleSphere.getColor(p, lookDirection);
                // c = blueSphere.getColor(p, lookDirection);

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
