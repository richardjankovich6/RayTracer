#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>


#include "primitive.h"
#include "sphere.h"
#include "triangle.h"


#include "nlohmann/json.hpp" // https://github.com/nlohmann/json

using Json = nlohmann::json;

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

        Json config;
        try {
            config = Json::parse(configFile, nullptr, true, true);
        } catch (const Json::exception& e) {
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
                }
                else if (primitiveJson.contains("type") && primitiveJson["type"] == "Triangle") {
                    Triangle* tri = parseTriangle(primitiveJson, light);
                    primitives.push_back(tri);
                }
                else {
                    throw std::runtime_error("Unknown primitive type: " + std::string(primitiveJson.value("type", "")));
                }
            } catch (const Json::exception& e) {
                throw std::runtime_error("Error parsing primitive at line " +
                                        //  std::to_string(primitiveJson.position()) + ": " +
                                         std::string(e.what()));
            }
        }

        return primitives;
    }

private:
    static Sphere* parseSphere(const Json& json, const Light* light) {
        auto sphere = new Sphere();

        // Required properties
        if (!json.contains("center") || !json["center"].is_array() ||
            json["center"].size() != 3) {
            throw std::runtime_error("Sphere must have a center with x, y, z coordinates");
        }

        sphere->setCenter(Vector3(
            json["center"][0].get<double>(),
            json["center"][1].get<double>(),
            json["center"][2].get<double>()
        ));

        sphere->setRadius(json["radius"].get<double>());

        sphere->setDiffiuseColor(Vector3(
            json["diffuseColor"][0].get<double>(),
            json["diffuseColor"][1].get<double>(),
            json["diffuseColor"][2].get<double>()
        ));
        sphere->setSpecularColor(Vector3(
            json["specularColor"][0].get<double>(),
            json["specularColor"][1].get<double>(),
            json["specularColor"][2].get<double>()
        ));


        // Set material properties
        float kDiffuse = 0.6, kSpecular = 0.7, kAmbient = 0.1;
        if (json.contains("kDiffuse") && json["kDiffuse"].is_number()) {
            kDiffuse = json["kDiffuse"].get<float>();
        }
        if (json.contains("kSpecular") && json["kSpecular"].is_number()) {
            kSpecular = json["kSpecular"].get<float>();
        }
        if (json.contains("kAmbient") && json["kAmbient"].is_number()) {
            kAmbient = json["kAmbient"].get<float>();
        }
        sphere->setKValues(kDiffuse, kSpecular, kAmbient);

        float kGloss = 1.0;
        if (json.contains("kGloss") && json["kGloss"].is_number()) {
            kGloss = json["kGloss"].get<float>();
        }
        sphere->setKGloss(kGloss);

        float reflectiveness = 0.0;
        if (json.contains("reflectiveness") && json["reflectiveness"].is_number()) {
            reflectiveness = json["reflectiveness"].get<float>();
        }
        sphere->setReflectiveness(reflectiveness);


        sphere->setLight(light);

        return sphere;
    }

    static Triangle* parseTriangle(const Json& json, const Light* light) {

        // Required properties - three points
        if (!json.contains("points") || !json["points"].is_array() ||
            json["points"].size() != 3) {
            throw std::runtime_error("Triangle must have exactly three points");
        }

        for (int i = 0; i < 3; ++i) {
            if (!json["points"][i].is_array() || json["points"][i].size() != 3) {
                throw std::runtime_error("Each point in a triangle must have x, y, z coordinates");
            }
        }

        Triangle* triangle = new Triangle(
            Point(json["points"][0][0].get<double>(), json["points"][0][1].get<double>(), json["points"][0][2].get<double>()),
            Point(json["points"][1][0].get<double>(), json["points"][1][1].get<double>(), json["points"][1][2].get<double>()),
            Point(json["points"][2][0].get<double>(), json["points"][2][1].get<double>(), json["points"][2][2].get<double>())
        );

        if (json.contains("normal")) {
            triangle->setNormal(Vector3<double>(
                json["normal"][0].get<double>(), 
                json["normal"][1].get<double>(), 
                json["normal"][2].get<double>()
            ));
        }

        // Diffuse color
        triangle->setDiffiuseColor(Vector3(
            json["diffuseColor"][0].get<double>(),
            json["diffuseColor"][1].get<double>(),
            json["diffuseColor"][2].get<double>()
        ));

        // Specular color
        triangle->setSpecularColor(Vector3(
            json["specularColor"][0].get<double>(),
            json["specularColor"][1].get<double>(),
            json["specularColor"][2].get<double>()
        ));

        float kGloss = 12.0;
        if (json.contains("kGloss") && json["kGloss"].is_number()) {
            kGloss = json["kGloss"].get<float>();
        }


        // Optional properties with defaults
        float kDiffuse = 0.6, kSpecular = 0.7, kAmbient = 0.1;
        if (json.contains("kDiffuse") && json["kDiffuse"].is_number()) {
            kDiffuse = json["kDiffuse"].get<float>();
        }
        if (json.contains("kSpecular") && json["kSpecular"].is_number()) {
            kSpecular = json["kSpecular"].get<float>();
        }
        if (json.contains("kAmbient") && json["kAmbient"].is_number()) {
            kAmbient = json["kAmbient"].get<float>();
        }

        float reflectiveness = 0.0;
        if (json.contains("reflectiveness") && json["reflectiveness"].is_number()) {
            reflectiveness = json["reflectiveness"].get<float>();
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
