// PrimitiveConfigParser.h
#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Assuming you have a 'Primitive' base class, Sphere, Triangle, Point, Vector3 defined elsewhere
#include "primitive.h"
#include "sphere.h"
#include "triangle.h"
#include "../point.h" // or however your geometry is defined

#include "../nlohmann/json.hpp" // The json library we will use
using nlohmann::json;

class PrimitiveConfigParser {
public:
    static std::vector<Primitive*> loadPrimitives(const std::string& filename) {
        try {
            // 1. Load and parse the file
            JsonReader reader;
            json j = reader.read(filename); 

            // 2. Create a vector to store raw pointers (heap allocated by us for safety)
            std::vector<Primitive*> primitives;

            // 3. Iterate over each primitive object in the JSON array
            for(const auto& primitive_data : j) {
                Primitive* p = nullptr;

                if(primitive_data.contains("type")) {
                    const std::string type = primitive_data["type"].get<std::string>();

                    if (type == "sphere") {
                        Sphere* s = new Sphere(); 
                        p = s; // Keep track of raw pointer
                        
                        // Parse sphere data
                        setSphereProperties(*s, primitive_data);
                        
                        primitives.push_back(p);
                    } 
                    else if (type == "triangle") {
                        Triangle* t = new Triangle(
                            Point(primitive_data["pointA"][0].get<double>(), 
                                 primitive_data["pointA"][1].get<double>(), 
                                 primitive_data["pointA"][2].get<double>()),
                            Point(primitive_data["pointB"][0].get<double>(), 
                                 primitive_data["pointB"][1].get<double>(), 
                                 primitive_data["pointB"][2].get<double>()),
                            Point(primitive_data["pointC"][0].get<double>(), 
                                 primitive_data["pointC"][1].get<double>(), 
                                 primitive_data["pointC"][2].get<double>())
                        );
                        // Note: Triangle usually takes constructor args. 
                        // If your Triangle uses setters like Sphere, adjust the logic here.

                        p = t;

                        // Parse triangle data (e.g., material properties)
                        setCommonProperties(*p, primitive_data);
                        
                        primitives.push_back(p);
                    } 
                    else {
                        throw std::runtime_error("Unknown primitive type: " + type);
                    }
                }
            }

            return primitives;
            
        } catch (const std::exception& e) {
            std::cerr << "Error loading primitives: " << e.what() << "\n";
            return {}; // Return empty on error
        }
    }

private:
    // Helper to handle common properties (K values, Colors)
    static void setCommonProperties(Primitive& prim, const json& data) {
        if(data.contains("kDiffiuse")) prim.setKDiffiuse(data["kDiffiuse"].get<float>());
        if(data.contains("kSpecular")) prim.setKSpecular(data["kSpecular"].get<float>());
        if(data.contains("KAmbient"))   prim.setKAmbient(data["KAmbient"].get<float>());
        if(data.contains("reflectiveness")) prim.setReflectiveness(data["reflectiveness"].get<float>());
        
        // Set Colors (assuming your Vector3 accepts array or specific setter)
        if(data.contains("diffiuseColor")) {
            auto c = data["diffiuseColor"].get<std::vector<double>>();
            // Adjust this to match your Color class constructor/setter exactly
            prim.setDiffiuseColor({(float)c[0], (float)c[1], (float)c[2]}); 
        }
        
        if(data.contains("specularColor")) {
             auto c = data["specularColor"].get<std::vector<double>>();
             prim.setSpecularColor({(float)c[0], (float)c[1], (float)c[2]});
        }
        
        if(data.contains("light")) {
            // Assuming 'light' is a global or passed object in your parser context,
            // otherwise this would require passing the Light object to the Parser.
            // prim.setLight(light); 
             // You might need to store light data in JSON instead of a pointer.
        }

        if(data.contains("KGloss")) prim.setKGloss(data["KGloss"].get<float>());
    }

    // Helper specific for Spheres
    static void setSphereProperties(Sphere& s, const json& data) {
        if(data.contains("center")) {
            auto p = data["center"].get<std::vector<double>>();
            s.setCenter(Vector3(p[0], p[1], p[2]));
        }
        if(data.contains("radius")) {
            s.setRadius(data["radius"].get<double>());
        }
        // Other sphere properties can be added here
    }
};
