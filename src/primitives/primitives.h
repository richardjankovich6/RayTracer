#pragma once

#include <vector>
#include <cmath>

#include "../color.h"
#include "../lights/light.h"
#include "../vector3.h"
#include "../ray.h"
#include "../point.h"

#ifndef PRIMITIVES_H
#define PRIMITIVES_H

enum PrimitiveType {
    none = 0,
    triangle = 1,
    sphere = 2
};

#define numMaterialValues 11
enum MaterialOffset : int {
    kDiffuse = 0,
    kSpecular = 1,
    kAmbient = 2,
    kGloss = 3,
    reflectiveness = 4,
    diffuseR = 5,
    diffuseG = 6,
    diffuseB = 7,
    specularR = 8,
    specularG = 9,
    specularB = 10
};

// struct align(64) Primitives {
struct Primitives {

    int numPrimitives;

    std::vector<double> materialValues;

    // std::vector<float> kDiffuse, kSpecular, kAmbient, kGloss, reflectiveness;
    // std::vector<PrimitiveType> pType;

    // std::vector<double> diffuseR, diffuseG, diffuseB;
    // std::vector<double> specularR, specularG, specularB;

    void initializeVectors(int size) {
        numPrimitives = size;
        materialValues = std::vector<double>(size * numMaterialValues);


        // kDiffuse = std::vector<float>(size);
        // kSpecular = std::vector<float>(size);
        // kAmbient = std::vector<float>(size);
        // kGloss = std::vector<float>(size);
        // reflectiveness = std::vector<float>(size);
        
        // diffuseR = std::vector<double>(size);
        // diffuseG = std::vector<double>(size);
        // diffuseB = std::vector<double>(size);
        // specularR = std::vector<double>(size);
        // specularG = std::vector<double>(size);
        // specularB = std::vector<double>(size);

        // pType = std::vector<PrimitiveType>(size);
    }

    /*
    void addPrimitive(float kDiffuse_, float kSpecular_, float kAmbient_, float kGloss_, float reflectiveness_, Color diffuseColor_, Color specularColor_) {
        // int index = kDiffuse.size();

        // Vector3<float> temp = unitVec<float>({kDiffuse_, kSpecular_, kAmbient_});
        // kDiffuse.push_back(temp[0]);
        // kSpecular.push_back(temp[1]);
        // kAmbient.push_back(temp[2]);

        // kGloss.push_back(kGloss_);
        // reflectiveness.push_back(reflectiveness_);

        // diffuseR.push_back(diffuseColor_[0]);
        // diffuseG.push_back(diffuseColor_[1]);
        // diffuseB.push_back(diffuseColor_[2]);

        // specularR.push_back(specularColor_[0]);
        // specularG.push_back(specularColor_[1]);
        // specularB.push_back(specularColor_[2]);
    }
    */

    void setPrimitive(int i, double kDiffuse_, double kSpecular_, double kAmbient_, double kGloss_, double reflectiveness_, Color diffuseColor_, Color specularColor_) {
        if (i >= numPrimitives) {
            return;
        }

        // Vector3<float> temp = unitVec<float>({kDiffuse_, kSpecular_, kAmbient_});
        // kDiffuse[i] = temp[0];
        // kSpecular[i] = temp[1];
        // kAmbient[i] = temp[2];

        // float totalK = kAmbient_ + kDiffuse_ + kSpecular_;
        // totalK = 1;
        // kDiffuse[i] = kDiffuse_ / totalK;
        // kSpecular[i] = kSpecular_ / totalK;
        // kAmbient[i] = kAmbient_ / totalK;
        // kGloss[i] = kGloss_;
        // reflectiveness[i] = reflectiveness_;

        // diffuseR[i] = diffuseColor_[0];
        // diffuseG[i] = diffuseColor_[1];
        // diffuseB[i] = diffuseColor_[2];

        // specularR[i] = specularColor_[0];
        // specularG[i] = specularColor_[1];
        // specularB[i] = specularColor_[2];


        double totalK = kAmbient_ + kDiffuse_ + kSpecular_;
        // totalK = 1;
        // materialValues[i + MaterialOffset::
        materialValues[i + MaterialOffset::kDiffuse] = kDiffuse_ / totalK;
        materialValues[i + MaterialOffset::kSpecular] = kSpecular_ / totalK;
        materialValues[i + MaterialOffset::kAmbient] = kAmbient_ / totalK;
        materialValues[i + MaterialOffset::kGloss] = kGloss_;
        materialValues[i + MaterialOffset::reflectiveness] = reflectiveness_;

        materialValues[i + MaterialOffset::diffuseR] = diffuseColor_[0];
        materialValues[i + MaterialOffset::diffuseG] = diffuseColor_[1];
        materialValues[i + MaterialOffset::diffuseB] = diffuseColor_[2];

        materialValues[i + MaterialOffset::specularR] = specularColor_[0];
        materialValues[i + MaterialOffset::specularG] = specularColor_[1];
        materialValues[i + MaterialOffset::specularB] = specularColor_[2];
    }
    
    Primitives() {};
    virtual ~Primitives() = default;

    Color phongShading(int index, Vector3<double> normal, Vector3<double> lookDirection, const Light* light, bool inShadow) const {
        
        if (index >= numPrimitives) { return Light::backgroundColor; }

        Color diffuseColor = Color(materialValues[index * numMaterialValues + MaterialOffset::diffuseR],
                                   materialValues[index * numMaterialValues + MaterialOffset::diffuseG],
                                   materialValues[index * numMaterialValues + MaterialOffset::diffuseB]);
        Color aColor = Light::ambientLight * materialValues[index * numMaterialValues + MaterialOffset::kAmbient] * diffuseColor;
        if (inShadow) {
            return aColor;
        }
        
        double dotProduct = dot(normal, light->getDirectionToLight());
        Color dColor = materialValues[index * numMaterialValues + MaterialOffset::kDiffuse] * light->getLightColor() * diffuseColor * fmax(0.0, dotProduct);

        Vector3 reflectVector = unitVec(2 * normal * light->getDirectionToLight());
        Color specularColor = Color(materialValues[index * numMaterialValues + MaterialOffset::specularR],
                                    materialValues[index * numMaterialValues + MaterialOffset::specularG],
                                    materialValues[index * numMaterialValues + MaterialOffset::specularB]);
        Color sColor = materialValues[index * numMaterialValues + MaterialOffset::kSpecular] * light->getLightColor() * specularColor * pow(fmax(0.0, dot(lookDirection, reflectVector)), materialValues[index * numMaterialValues + MaterialOffset::kGloss]);

        // return sColor;
        return aColor + sColor + dColor;
    }

    inline Color getBackgroundColor() const { return Light::backgroundColor; }

    // virtual Color getColor(int i, Point p, Vector3<double> v, const Light* light, bool inShadow) const {
    //     return getBackgroundColor();
    // }
};

#endif

#ifndef SPHERES_H
#define SPHERES_H

#define numSphereValues 4
enum SphereValueOffset {
    centerX = 0,
    centerY = 1,
    centerZ = 2,
    radius = 3
};

struct Spheres : public Primitives {
    std::vector<double> sphereValues;

    // std::vector<double> centerX, centerY, centerZ;
    // std::vector<double> radius;

    void initializeVectors(int size) {
        sphereValues = std::vector<double>(size * numSphereValues);
        // centerX = std::vector<double>(size);
        // centerY = std::vector<double>(size);
        // centerZ = std::vector<double>(size);
        // radius = std::vector<double>(size);
        Primitives::initializeVectors(size);
    }

    Spheres() : Primitives() {};

    /*
    void addSphere(Vector3<double> center_, double radius_, float kDiffuse_, float kSpecular_, float reflectiveness_, float kAmbient_, float kGloss_, Color diffuseColor_, Color specularColor_) {
        centerX.push_back(center_[0]);
        centerY.push_back(center_[1]);
        centerZ.push_back(center_[2]);
        radius.push_back(radius_);
        addPrimitive(kDiffuse_, kSpecular_, kAmbient_, kGloss_, reflectiveness_, diffuseColor_, specularColor_);
        pType.push_back(PrimitiveType::sphere);
    }
    */

    void setSphere(int i, Vector3<double> center_, double radius_, float kDiffuse_, float kSpecular_, float reflectiveness_, float kAmbient_, double kGloss_, Color diffuseColor_, Color specularColor_) {
        sphereValues[i * numSphereValues + SphereValueOffset::centerX] = center_[0];
        sphereValues[i * numSphereValues + SphereValueOffset::centerY] = center_[1];
        sphereValues[i * numSphereValues + SphereValueOffset::centerZ] = center_[2];
        sphereValues[i * numSphereValues + SphereValueOffset::radius] = radius_;
        setPrimitive(i, kDiffuse_, kSpecular_, kAmbient_, kGloss_, reflectiveness_, diffuseColor_, specularColor_);   
    }

    inline Vector3<double> getReflectVec(Vector3<double> normal, Vector3<double> directionToLight) {
        return unitVec(2 * normal * dot(normal, directionToLight) - directionToLight);
    }

    inline Point getCenter(int i) const { return Point(sphereValues[i * numSphereValues + SphereValueOffset::centerX],
                                                       sphereValues[i * numSphereValues + SphereValueOffset::centerY],
                                                       sphereValues[i * numSphereValues + SphereValueOffset::centerZ]); }

    Vector3<double> getNormal(int i, Point p) const {
        return (p - getCenter(i)) / sphereValues[i * numSphereValues + SphereValueOffset::radius];
    }

    bool intersetRay(int i, Point& p, const Ray ray) const {
        Vector3 oc = getCenter(i) - ray.origin();
        bool bInside = oc.length() < sphereValues[i * numSphereValues + SphereValueOffset::radius];

        double tca = dot(oc, ray.direction());

        if (!bInside && (tca < 0)) {
            return false;
        }

        double thcSquare = pow(sphereValues[i * numSphereValues + SphereValueOffset::radius], 2) - oc.lengthSquared() + pow(tca, 2);
        if (thcSquare < 0) {
            return false;
        }

        double t = tca;
        t += bInside ? sqrt(thcSquare) : -sqrt(thcSquare);
        p = ray.origin() + ray.direction() * t;
        return true;
    };

    Color getColor(int i, Point p, Vector3<double> v, const Light* light, bool inShadow) const {
        Vector3 normal = getNormal(i, p);
        return phongShading(i, normal, v, light, inShadow);
    }

};

#endif

#ifndef TRIANGLES_H
#define TRIANGLES_H

#define numTriangleValues 14
enum TriangleValuesOffset {
    p0x = 0,
    p0y = 1,
    p0z = 2,
    p1x = 3,
    p1y = 4,
    p1z = 5,
    p2x = 6,
    p2y = 7,
    p2z = 8,
    normalX = 9,
    normalY = 10,
    normalZ = 11,
    detriment = 12,
    doubleSided = 13
};


struct Triangles : public Primitives {

    std::vector<double>triangleValues;

    // std::vector<double> p0x, p0y, p0z;
    // std::vector<double> p1x, p1y, p1z;
    // std::vector<double> p2x, p2y, p2z;
    
    // std::vector<double> normalX, normalY, normalZ;

    // std::vector<double> detriment;
    // std::vector<bool> doubleSided;

    Triangles() : Primitives() {};


    /*
    void addTriangle(Point p0, Point p1, Point p2, Vector3<double> normal, bool doubleSided_, float kDiffuse_, float kSpecular_, float kAmbient_, float kGloss_, float reflectiveness_, Color diffuseColor_, Color specularColor_) {
        p0x.push_back(p0[0]);
        p0y.push_back(p0[1]);
        p0z.push_back(p0[2]);
        p1x.push_back(p0[0]);
        p1y.push_back(p0[1]);
        p1z.push_back(p0[2]);
        p2x.push_back(p0[0]);
        p2y.push_back(p0[1]);
        p2z.push_back(p0[2]);

        Vector3<double> temp = normalize(normal);
        normalX.push_back(temp[0]);
        normalY.push_back(temp[1]);
        normalZ.push_back(temp[2]);
        detriment.push_back(-dot(temp, p0));

        doubleSided.push_back(doubleSided_);

        addPrimitive(kDiffuse_, kSpecular_, kAmbient_, kGloss_, reflectiveness_, diffuseColor_, specularColor_);
        pType.push_back(PrimitiveType::triangle);
    };
    */

    void setTriangle(int i, Point p0, Point p1, Point p2, Vector3<double> normal, bool doubleSided_, float kDiffuse_, float kSpecular_, float kAmbient_, float kGloss_, float reflectiveness_, Color diffuseColor_, Color specularColor_) {
        triangleValues[i * numTriangleValues + TriangleValuesOffset::p0x] = p0[0];
        triangleValues[i * numTriangleValues + TriangleValuesOffset::p0y] = p0[1];
        triangleValues[i * numTriangleValues + TriangleValuesOffset::p0z] = p0[2];
        triangleValues[i * numTriangleValues + TriangleValuesOffset::p1x] = p1[0];
        triangleValues[i * numTriangleValues + TriangleValuesOffset::p1y] = p1[1];
        triangleValues[i * numTriangleValues + TriangleValuesOffset::p1z] = p1[2];
        triangleValues[i * numTriangleValues + TriangleValuesOffset::p2x] = p2[0];
        triangleValues[i * numTriangleValues + TriangleValuesOffset::p2y] = p2[1];
        triangleValues[i * numTriangleValues + TriangleValuesOffset::p2z] = p2[2];

        Vector3<double> temp = normalize(normal);
        triangleValues[i * numTriangleValues + TriangleValuesOffset::normalX] = temp[0];
        triangleValues[i * numTriangleValues + TriangleValuesOffset::normalY] = temp[1];
        triangleValues[i * numTriangleValues + TriangleValuesOffset::normalZ] = temp[2];
        triangleValues[i * numTriangleValues + TriangleValuesOffset::detriment] = -dot(temp, p0);

        triangleValues[i * numTriangleValues + TriangleValuesOffset::doubleSided] = doubleSided_ ? 1.0 : 0.0;

        setPrimitive(i, kDiffuse_, kSpecular_, kAmbient_, kGloss_, reflectiveness_, diffuseColor_, specularColor_);
        
    };

    void initializeVectors(int size) {
        triangleValues = std::vector<double>(size * numTriangleValues);

        // p0x = std::vector<double>(size);
        // p0y = std::vector<double>(size);
        // p0z = std::vector<double>(size);
        // p1x = std::vector<double>(size);
        // p1y = std::vector<double>(size);
        // p1z = std::vector<double>(size);
        // p2x = std::vector<double>(size);
        // p2y = std::vector<double>(size);
        // p2z = std::vector<double>(size);

        // normalX = std::vector<double>(size);
        // normalY = std::vector<double>(size);
        // normalZ = std::vector<double>(size);
        // detrminent = std::vector<double>(size);

        // doubleSided = std::vector<bool>(size);

        Primitives::initializeVectors(size);
    }

    void setNormal(int i, const Vector3<double> v) {
        Vector3<double> temp = normalize(v);
        triangleValues[i * numTriangleValues + TriangleValuesOffset::normalX] = temp[0];
        triangleValues[i * numTriangleValues + TriangleValuesOffset::normalY] = temp[1];
        triangleValues[i * numTriangleValues + TriangleValuesOffset::normalZ] = temp[2];
        triangleValues[i * numTriangleValues + TriangleValuesOffset::detriment] = -dot(temp, 
            Point(triangleValues[i * numTriangleValues + TriangleValuesOffset::p0x],
                    triangleValues[i * numTriangleValues + TriangleValuesOffset::p0y],
                    triangleValues[i * numTriangleValues +  TriangleValuesOffset::p0z])
                );
    };

    bool intersetRay(int i, Point& p, const Ray ray) const {
        Vector3<double> normal = getNormal(i);
        double vd = dot(normal, ray.direction());
        if (vd == 0) {
            return false;
        }
        if (triangleValues[i * numTriangleValues + TriangleValuesOffset::doubleSided] != 0 && vd > 0) {
            return false;
        } 

        double vo = -(dot(normal, ray.origin()) + triangleValues[i * numTriangleValues + TriangleValuesOffset::detriment]);
        double t = vo / vd;
        
        if (t < 0) {
            return false;
        }
        
        p = ray.at(t);

        Point p0 = Point(triangleValues[i * numTriangleValues + TriangleValuesOffset::p0x], triangleValues[i * numTriangleValues + TriangleValuesOffset::p0y], triangleValues[i * numTriangleValues + TriangleValuesOffset::p0z]);
        Point p1 = Point(triangleValues[i * numTriangleValues + TriangleValuesOffset::p1x], triangleValues[i * numTriangleValues + TriangleValuesOffset::p1y], triangleValues[i * numTriangleValues + TriangleValuesOffset::p1z]);
        Point p2 = Point(triangleValues[i * numTriangleValues + TriangleValuesOffset::p2x], triangleValues[i * numTriangleValues + TriangleValuesOffset::p2y], triangleValues[i * numTriangleValues + TriangleValuesOffset::p2z]);

        bool dot1 = dot(normal, cross(p1 - p0, p - p0)) > 0;
        bool dot2 = dot(normal, cross(p2 - p1, p - p1)) > 0;
        bool dot3 = dot(normal, cross(p0 - p2, p - p2)) > 0;
        bool intersect = dot1 && dot2 && dot3;

        return intersect;
    }

    inline Vector3<double> getNormal(int i) const { return Vector3<double>(triangleValues[i * numTriangleValues + TriangleValuesOffset::normalX], triangleValues[i * numTriangleValues + TriangleValuesOffset::normalY], triangleValues[i * numTriangleValues + TriangleValuesOffset::normalZ]); }
    Color getColor(int i, Vector3<double> v, const Light* light, bool inShadow) const {
        return phongShading(i, getNormal(i), v, light, inShadow);
    }
    
};

#endif

