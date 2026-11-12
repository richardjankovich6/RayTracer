#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>


#include "../primitives/primitives.h"
#include "../lights/light.h"
#include "../lights/pointLight.h"
#include "../lights/directionalLight.h"
#include "../camera.h"


#include "../nlohmann/json.hpp" // https://github.com/nlohmann/json
#include "../color.h"


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

    static void parsePrimitives(Triangles& triangles, Spheres& spheres, const std::string& filename) {
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

        std::vector<nlohmann::basic_json<>> sphereJsons;
        std::vector<nlohmann::basic_json<>> triangleJsons;

        for (const auto& primitiveJson : config["primitives"]) {
            try {
                if (primitiveJson.contains("type") && primitiveJson["type"] == "Sphere") {
                    // sphereCount++;
                    sphereJsons.push_back(primitiveJson);
                    // Sphere* sphere = parseSphere(primitiveJson);
                    // primitives.push_back(sphere);
                }
                else if (primitiveJson.contains("type") && primitiveJson["type"] == "Triangle") {
                    // triangleCount++;
                    triangleJsons.push_back(primitiveJson);
                    // Triangle* tri = parseTriangle(primitiveJson);
                    // primitives.push_back(tri);
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
        triangles.initializeVectors(triangleJsons.size());
        for (int i = 0; i < triangleJsons.size(); i++) {
            // parse json
            parseTriangle(i, triangles, triangleJsons[i]);
            // triangles.setriangle(i, Point p0, Point p1, Point p2, Vector3<double> normal, bool doubleSided_, float kDiffuse_, float kSpecular_, float kAmbient_, float kGloss_, float reflectiveness_, Color diffuseColor_, Color specularColor_)
        }

        spheres.initializeVectors(sphereJsons.size());
        for (int i = 0; i < sphereJsons.size(); i++) {
            parseSphere(i, spheres, sphereJsons[i]);
            // parse json
            // spheres.setSphere(i, Vector3<double> center_, double radius_, float kDiffuse_, float kSpecular_, float reflectiveness_, float kAmbient_, double kGloss_, Color diffuseColor_, Color specularColor_);
        }
    }

private:

    static void parseSphere(int index, Spheres& spheres, const Json& json) {
        // Sphere* sphere = new Sphere();

        // Required properties
        if (!json.contains("center") || !json["center"].is_array() ||
            json["center"].size() != 3) {
            throw std::runtime_error("Sphere must have a center with x, y, z coordinates");
        }

        Point center = Point(json["center"][0].get<double>(), json["center"][1].get<double>(), json["center"][2].get<double>());

        double radius = json["radius"].get<double>();

        Color diffuseColor = Color(json["diffuseColor"][0].get<double>(), json["diffuseColor"][1].get<double>(), json["diffuseColor"][2].get<double>());
        Color specularColor = Color(json["specularColor"][0].get<double>(), json["specularColor"][1].get<double>(), json["specularColor"][2].get<double>());

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
        // sphere->setKValues(kDiffuse, kSpecular, kAmbient);

        float kGloss = 1.0;
        if (json.contains("kGloss") && json["kGloss"].is_number()) {
            kGloss = json["kGloss"].get<float>();
        }
        // sphere->setKGloss(kGloss);

        float reflectiveness = 0.0;
        if (json.contains("reflectiveness") && json["reflectiveness"].is_number()) {
            reflectiveness = json["reflectiveness"].get<float>();
        }
        // sphere->setReflectiveness(reflectiveness);


        spheres.setSphere(index, center, radius, kDiffuse, kSpecular, reflectiveness, kAmbient, kGloss, diffuseColor, specularColor);


        // sphere->setLight(light);

        // return sphere;
    }

    static void parseTriangle(int index, Triangles& triangles, const Json& json) {

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

        Point p0 = Point(json["points"][0][0].get<double>(), json["points"][0][1].get<double>(), json["points"][0][2].get<double>());
        Point p1 = Point(json["points"][1][0].get<double>(), json["points"][1][1].get<double>(), json["points"][1][2].get<double>());
        Point p2 = Point(json["points"][2][0].get<double>(), json["points"][2][1].get<double>(), json["points"][2][2].get<double>());

        

        Vector3<double> normal;
        if (json.contains("normal")) {
            normal = Vector3<double>(json["normal"][0].get<double>(), json["normal"][1].get<double>(), json["normal"][2].get<double>());
        }
        else {
            Vector3<double> v1 = p2 - p1;
            Vector3<double> v2 = p0 - p1;
            normal = unitVec(cross(v1, v2));
        }

        Color diffuseColor = Color(json["diffuseColor"][0].get<double>(), json["diffuseColor"][1].get<double>(), json["diffuseColor"][2].get<double>());
        Color specularColor = Color(json["specularColor"][0].get<double>(), json["specularColor"][1].get<double>(), json["specularColor"][2].get<double>());

        float kGloss = 0.0;
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

        bool doubleSided = true;
        if (json.contains("doubleSided") && json["doubleSided"].is_boolean()) {
            reflectiveness = json["doubleSided"].get<bool>();
        }

        triangles.setTriangle(index, p0, p1, p2, normal, doubleSided, kDiffuse, kSpecular, kAmbient, kGloss, reflectiveness, diffuseColor, specularColor);

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
