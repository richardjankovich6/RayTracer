#include <iostream>
#include <vector>
#include <string>
#include <fstream>
// #include <memory>
#include <stdexcept>


#include "primitive.h"
#include "sphere.h"
#include "triangle.h"


#include "nlohmann/json.hpp" // https://github.com/nlohmann/json

using json = nlohmann::json;

// Forward declarations (assuming these exist in your code)
class Sphere;
class Triangle;
class Primitive;
class Light;

class JsonPrimitiveParser {
public:
    static std::vector<Primitive*> parsePrimitives(const std::string& filename, const Light* light) {
        // Read the JSON file
        std::ifstream configFile(filename);
        if (!configFile.is_open()) {
            throw std::runtime_error("Could not open config file: " + filename);
        }

        json config;
        try {
            config = json::parse(configFile, nullptr, true, true);
        } catch (const json::exception& e) {
            throw std::runtime_error("Invalid JSON in config file: " + std::string(e.what()));
        }

        // Validate the root structure
        if (!config.is_object() || !config.contains("primitives")) {
            throw std::runtime_error("Config file must contain a 'primitives' array");
        }

        // std::vector<Primitive*> primitives;
        std::vector<Primitive*> primitives;

        for (const auto& primitiveJson : config["primitives"]) {
            try {
                if (primitiveJson.contains("type") && primitiveJson["type"] == "Sphere") {
                    Sphere* sphere = parseSphere(primitiveJson, light);
                    primitives.push_back(sphere);
                    // std::cout << "sphere made\n";
                }
                else if (primitiveJson.contains("type") && primitiveJson["type"] == "Triangle") {
                    Triangle* tri = parseTriangle(primitiveJson, light);
                    primitives.push_back(tri);
                    // std::cout << "tri made\n";
                }
                else {
                    throw std::runtime_error("Unknown primitive type: " + std::string(primitiveJson.value("type", "")));
                }
            } catch (const json::exception& e) {
                throw std::runtime_error("Error parsing primitive at line " +
                                        //  std::to_string(primitiveJson.position()) + ": " +
                                         std::string(e.what()));
            }
        }

        return primitives;
    }

private:
    static Sphere* parseSphere(const json& sphereJson, const Light* light) {
        auto sphere = new Sphere();

        // Required properties
        if (!sphereJson.contains("center") || !sphereJson["center"].is_array() ||
            sphereJson["center"].size() != 3) {
            throw std::runtime_error("Sphere must have a center with x, y, z coordinates");
        }

        sphere->setCenter(Vector3(
            sphereJson["center"][0].get<double>(),
            sphereJson["center"][1].get<double>(),
            sphereJson["center"][2].get<double>()
        ));

        if (!sphereJson.contains("radius") || !sphereJson["radius"].is_number()) {
            throw std::runtime_error("Sphere must have a radius value");
        }
        sphere->setRadius(sphereJson["radius"].get<double>());

        // Diffuse color
        if (!sphereJson.contains("diffuseColor") || !sphereJson["diffuseColor"].is_array() ||
            sphereJson["diffuseColor"].size() != 3) {
            throw std::runtime_error("Sphere must have a diffuseColor with r, g, b values");
        }
        sphere->setDiffiuseColor(Vector3(
            sphereJson["diffuseColor"][0].get<double>(),
            sphereJson["diffuseColor"][1].get<double>(),
            sphereJson["diffuseColor"][2].get<double>()
        ));

        // Specular color
        if (!sphereJson.contains("specularColor") || !sphereJson["specularColor"].is_array() ||
            sphereJson["specularColor"].size() != 3) {
            throw std::runtime_error("Sphere must have a specularColor with r, g, b values");
        }
        sphere->setSpecularColor(Vector3(
            sphereJson["specularColor"][0].get<double>(),
            sphereJson["specularColor"][1].get<double>(),
            sphereJson["specularColor"][2].get<double>()
        ));

        float kGloss = 2.0;
        if (sphereJson.contains("kGloss") && sphereJson["kGloss"].is_number()) {
            kGloss = sphereJson["kGloss"].get<float>();
        }

        float kDiffuse = 0.6, kSpecular = 0.7, kAmbient = 0.1;
        if (sphereJson.contains("kDiffuse") && sphereJson["kDiffuse"].is_number()) {
            kDiffuse = sphereJson["kDiffuse"].get<float>();
        }
        if (sphereJson.contains("kSpecular") && sphereJson["kSpecular"].is_number()) {
            kSpecular = sphereJson["kSpecular"].get<float>();
        }
        if (sphereJson.contains("kAmbient") && sphereJson["kAmbient"].is_number()) {
            kAmbient = sphereJson["kAmbient"].get<float>();
        }

        float reflectiveness = 0.0;
        if (sphereJson.contains("reflectiveness") && sphereJson["reflectiveness"].is_number()) {
            reflectiveness = sphereJson["reflectiveness"].get<float>();
        }


        sphere->setKValues(kDiffuse, kSpecular, kAmbient);

        // Set material properties
        sphere->setReflectiveness(reflectiveness);
        sphere->setKGloss(kGloss);

        // Set light
        sphere->setLight(light);

        return sphere;
    }

    static Triangle* parseTriangle(const json& triangleJson, const Light* light) {
        

        // Required properties - three points
        if (!triangleJson.contains("points") || !triangleJson["points"].is_array() ||
            triangleJson["points"].size() != 3) {
            throw std::runtime_error("Triangle must have exactly three points");
        }

        for (int i = 0; i < 3; ++i) {
            if (!triangleJson["points"][i].is_array() || triangleJson["points"][i].size() != 3) {
                throw std::runtime_error("Each point in a triangle must have x, y, z coordinates");
            }
        }

        Triangle* triangle = new Triangle(
            Point(triangleJson["points"][0][0].get<double>(), triangleJson["points"][0][1].get<double>(), triangleJson["points"][0][2].get<double>()),
            Point(triangleJson["points"][1][0].get<double>(), triangleJson["points"][1][1].get<double>(), triangleJson["points"][1][2].get<double>()),
            Point(triangleJson["points"][2][0].get<double>(), triangleJson["points"][2][1].get<double>(), triangleJson["points"][2][2].get<double>())
        );

        // Diffuse color
        triangle->setDiffiuseColor(Vector3(
            triangleJson["diffuseColor"][0].get<double>(),
            triangleJson["diffuseColor"][1].get<double>(),
            triangleJson["diffuseColor"][2].get<double>()
        ));

        // Specular color
        triangle->setSpecularColor(Vector3(
            triangleJson["specularColor"][0].get<double>(),
            triangleJson["specularColor"][1].get<double>(),
            triangleJson["specularColor"][2].get<double>()
        ));

        float kGloss = 12.0;
        if (triangleJson.contains("kGloss") && triangleJson["kGloss"].is_number()) {
            kGloss = triangleJson["kGloss"].get<float>();
        }


        // Optional properties with defaults
        float kDiffuse = 0.6, kSpecular = 0.7, kAmbient = 0.1;
        if (triangleJson.contains("kDiffuse") && triangleJson["kDiffuse"].is_number()) {
            kDiffuse = triangleJson["kDiffuse"].get<float>();
        }
        if (triangleJson.contains("kSpecular") && triangleJson["kSpecular"].is_number()) {
            kSpecular = triangleJson["kSpecular"].get<float>();
        }
        if (triangleJson.contains("kAmbient") && triangleJson["kAmbient"].is_number()) {
            kAmbient = triangleJson["kAmbient"].get<float>();
        }

        float reflectiveness = 0.0;
        if (triangleJson.contains("reflectiveness") && triangleJson["reflectiveness"].is_number()) {
            reflectiveness = triangleJson["reflectiveness"].get<float>();
        }


        triangle->setKValues(kDiffuse, kSpecular, kAmbient);

        // Set material properties
        triangle->setReflectiveness(reflectiveness);
        triangle->setKGloss(kGloss);

        // Set light
        triangle->setLight(light);

        return triangle;
    }
};
