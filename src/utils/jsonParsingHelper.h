#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>


#include "../primitives/primitive.h"
#include "../primitives/sphere.h"
#include "../primitives/triangle.h"
#include "../lights/light.h"
#include "../lights/pointLight.h"
#include "../lights/directionalLight.h"
#include "../camera.h"


#include "../nlohmann/json.hpp" // https://github.com/nlohmann/json
#include "color.h"

// using nlohmann::literals::operator ""_json;
// using namespace nlohmann::literals;
// using namespace nlohmann::json_literals;
// using namespace nlohmann::literals::json_literals;
// using namespace nlohmann;

using Json = nlohmann::json;

class JsonParsingHelper {
public:

    static Camera parseCameraJson(const std::string& file) {

        std::ifstream configFile(file);
        if (!configFile.is_open()) {
            throw std::runtime_error("Could not open config file: " + file);
        }
        Json config;
        try {
            config = Json::parse(configFile, nullptr, true, true);
        } catch (const Json::exception& e) {
            throw std::runtime_error("Invalid JSON in config file: " + std::string(e.what()));
        }

        if(!config.contains("camera")) {
            throw std::runtime_error("json must contain \"camera\"");
        }

        auto camJson = config["camera"];
        Point lookAt = Point(
            camJson["lookAt"][0].get<double>(),
            camJson["lookAt"][1].get<double>(),
            camJson["lookAt"][2].get<double>());
        Point lookFrom = Point(
            camJson["lookFrom"][0].get<double>(),
            camJson["lookFrom"][1].get<double>(),
            camJson["lookFrom"][2].get<double>());
        Vector3<double> lookUp = Vector3<double>(
            camJson["lookFrom"][0].get<double>(),
            camJson["lookFrom"][1].get<double>(),
            camJson["lookFrom"][2].get<double>());
        int width = camJson["width"].get<int>();
        int height = camJson["height"].get<int>();
        double fov = camJson["fov"].get<int>();
        return Camera(lookAt, lookFrom, lookUp, width, height, fov);
    }

    static void parseLights(std::vector<Light*>& lights, const std::string& filename) {
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
        if (!config.is_object() || !config.contains("lights")) {
            throw std::runtime_error("Config file must contain a 'lights' array");
        }

        for (const auto& lightJson : config["lights"]) {
            try {
                if (lightJson.contains("type") && lightJson["type"] == "DirectionalLight") {
                    DirectionalLight* directionalLight = parseDirectionalLight(lightJson);
                    lights.push_back(directionalLight);
                }
                else if (lightJson.contains("type") && lightJson["type"] == "PointLight") {
                    PointLight* pointLight = parsePointLight(lightJson);
                    lights.push_back(pointLight);
                }
                else {
                    throw std::runtime_error("Unknown light type: " + std::string(lightJson.value("type", "")));
                }
            } catch (const Json::exception& e) {
                throw std::runtime_error("Error parsing light " +
                                        //  std::to_string(primitiveJson.position()) + ": " +
                                         std::string(e.what()));
            }
        }

        if (config.contains("ambientLight")) {
            Light::ambientLight = Color(
                config["ambientLight"][0].get<double>(),
                config["ambientLight"][1].get<double>(),
                config["ambientLight"][2].get<double>());
        }
        if (config.contains("backgroundColor")) {
            Light::backgroundColor = Color(
                config["backgroundColor"][0].get<double>(),
                config["backgroundColor"][1].get<double>(),
                config["backgroundColor"][2].get<double>());
        }
    }


    static void parsePrimitives(std::vector<Primitive*>& primitives, const std::string& filename) {
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

        for (const auto& primitiveJson : config["primitives"]) {
            try {
                if (primitiveJson.contains("type") && primitiveJson["type"] == "Sphere") {
                    Sphere* sphere = parseSphere(primitiveJson);
                    primitives.push_back(sphere);
                }
                else if (primitiveJson.contains("type") && primitiveJson["type"] == "Triangle") {
                    Triangle* tri = parseTriangle(primitiveJson);
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
    }

private:
    static Sphere* parseSphere(const Json& json) {
        Sphere* sphere = new Sphere();

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


        // sphere->setLight(light);

        return sphere;
    }

    static Triangle* parseTriangle(const Json& json) {

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
        // triangle->setLight(light);

        return triangle;
    }

    static PointLight* parsePointLight(const Json& json) {
        Point location = Point(
            json["location"][0].get<double>(),
            json["location"][1].get<double>(),
            json["location"][2].get<double>());
        Color color = Color(
            json["color"][0].get<double>(),
            json["color"][1].get<double>(),
            json["color"][2].get<double>());
        double intensity = 1.0;
        if(json.contains("intensity")) {
            intensity = json["intensity"].get<double>();
        }

        PointLight* light = new PointLight(color, location, intensity);
        return light;
    }

    static DirectionalLight* parseDirectionalLight(const Json& json) {

        Vector3<double> directionToLight = Vector3<double>(
            json["directionToLight"][0].get<double>(),
            json["directionToLight"][1].get<double>(),
            json["directionToLight"][2].get<double>());
        Color color = Color(
            json["color"][0].get<double>(),
            json["color"][1].get<double>(),
            json["color"][2].get<double>());
        
        DirectionalLight* light = new DirectionalLight(color, directionToLight);
        return light;
    }
};
