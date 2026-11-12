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

struct Primitives {

    std::vector<float> kDiffuse, kSpecular, kAmbient, kGloss, reflectiveness;
    std::vector<PrimitiveType> pType;

    std::vector<double> diffuseR, diffuseG, diffuseB;
    std::vector<double> specularR, specularG, specularB;

    void initializeVectors(int size) {
        kDiffuse = std::vector<float>(size);
        kSpecular = std::vector<float>(size);
        kAmbient = std::vector<float>(size);
        kGloss = std::vector<float>(size);
        reflectiveness = std::vector<float>(size);
        
        diffuseR = std::vector<double>(size);
        diffuseG = std::vector<double>(size);
        diffuseB = std::vector<double>(size);
        specularR = std::vector<double>(size);
        specularG = std::vector<double>(size);
        specularB = std::vector<double>(size);

        pType = std::vector<PrimitiveType>(size);
    }

    void addPrimitive(float kDiffuse_, float kSpecular_, float kAmbient_, float kGloss_, float reflectiveness_, Color diffuseColor_, Color specularColor_) {
        // int index = kDiffuse.size();

        Vector3<float> temp = unitVec<float>({kDiffuse_, kSpecular_, kAmbient_});

        kDiffuse.push_back(temp[0]);
        kSpecular.push_back(temp[1]);
        kAmbient.push_back(temp[2]);

        diffuseR.push_back(diffuseColor_[0]);
        diffuseG.push_back(diffuseColor_[1]);
        diffuseB.push_back(diffuseColor_[2]);

        specularR.push_back(specularColor_[0]);
        specularG.push_back(specularColor_[1]);
        specularB.push_back(specularColor_[2]);
    }

    void setPrimitive(int i, float kDiffuse_, float kSpecular_, float kAmbient_, float kGloss_, float reflectiveness_, Color diffuseColor_, Color specularColor_) {
        // int index = kDiffuse.size();

        if (i >= kDiffuse.size()) {
            return;
        }

        Vector3<float> temp = unitVec<float>({kDiffuse_, kSpecular_, kAmbient_});

        kDiffuse[i] = temp[0];
        kSpecular[i] = temp[1];
        kAmbient[i] = temp[2];

        diffuseR[i] = diffuseColor_[0];
        diffuseG[i] = diffuseColor_[1];
        diffuseB[i] = diffuseColor_[2];

        specularR[i] = specularColor_[0];
        specularG[i] = specularColor_[1];
        specularB[i] = specularColor_[2];
    }
    
    Primitives() {};
    virtual ~Primitives() = default;

    // Color phongShading(Vector3<double> normal, Vector3<double> lookDirection, const Light* light, bool inShadow) const;

    Color phongShading(int index, Vector3<double> normal, Vector3<double> lookDirection, const Light* light, bool inShadow) const {
        
        if (index >= kDiffuse.size()) { return Light::backgroundColor; }

        Color diffuseColor = Color(diffuseR[index], diffuseG[index], diffuseB[index]);
        Color aColor = Light::ambientLight * kAmbient[index] * diffuseColor;
        if (inShadow) {
            return aColor;
        }
        
        double dotProduct = dot(normal, light->getDirectionToLight());
        Color dColor = kDiffuse[index] * light->getLightColor() * diffuseColor * fmax(0.0, dotProduct);

        Vector3 reflectVector = unitVec(2 * normal * light->getDirectionToLight());
        Color specularColor = Color(specularR[index], specularG[index], specularB[index]);
        Color sColor = kSpecular[index] * light->getLightColor() * specularColor * pow(fmax(0.0, dot(lookDirection, reflectVector)), kGloss[index]);

        return aColor + sColor + dColor;
    }

    virtual void removeIndex(int index) {
        // kDiffuse. = temp[0];
        // kSpecular[i] = temp[1];
        // kAmbient[i] = temp[2];

        // diffuseR[i] = diffuseColor_[0];
        // diffuseG[i] = diffuseColor_[1];
        // diffuseB[i] = diffuseColor_[2];

        // specularR[i] = specularColor_[0];
        // specularG[i] = specularColor_[1];
        // specularB[i] = specularColor_[2];
    }

    inline Color getBackgroundColor() const { return Light::backgroundColor; }

    virtual bool intersetRay(int i, Point& v, Ray ray) const = 0;

    virtual Color getColor(int i, Point p, Vector3<double> v, const Light* light, bool inShadow) const {
        return getBackgroundColor();
    }

    virtual Vector3<double> getNormal(Point p = Point()) const {
        return Vector3<double>();
    }
};

#endif

#ifndef SPHERES_H
#define SPHERES_H

struct Spheres : public Primitives {

    std::vector<double> centerX, centerY, centerZ;
    std::vector<double> radius;

    void initializeVectors(int size) {
        centerX = std::vector<double>(size);
        centerY = std::vector<double>(size);
        centerZ = std::vector<double>(size);
        radius = std::vector<double>(size);
        Primitives::initializeVectors(size);
    }

    Spheres() : Primitives() {};

    void addSphere(Vector3<double> center_, double radius_, float kDiffuse_, float kSpecular_, float reflectiveness_, float kAmbient_, float kGloss_, Color diffuseColor_, Color specularColor_) {
        centerX.push_back(center_[0]);
        centerY.push_back(center_[1]);
        centerZ.push_back(center_[2]);
        radius.push_back(radius_);
        addPrimitive(kDiffuse_, kSpecular_, kAmbient_, kGloss_, reflectiveness_, diffuseColor_, specularColor_);
        pType.push_back(PrimitiveType::sphere);
    }

    void setSphere(int i, Vector3<double> center_, double radius_, float kDiffuse_, float kSpecular_, float reflectiveness_, float kAmbient_, double kGloss_, Color diffuseColor_, Color specularColor_) {
        centerX[i] = center_[0];
        centerY[i] = center_[1];
        centerZ[i] = center_[2];
        radius[i] = radius_;
        setPrimitive(i, kDiffuse_, kSpecular_, kAmbient_, kGloss_, reflectiveness_, diffuseColor_, specularColor_);
        pType[i] = PrimitiveType::sphere;
    }

    inline Vector3<double> getReflectVec(Vector3<double> normal, Vector3<double> directionToLight) {
        return unitVec(2 * normal * dot(normal, directionToLight) - directionToLight);
    }

    inline Point getCenter(int i) const { return Point(centerX[i], centerY[i], centerZ[i]); }

    Vector3<double> getNormal(int i, Point p) const {
        return (p - getCenter(i)) / radius[i];
    }

    bool intersetRay(int i, Point& p, const Ray ray) const override {
        Vector3 oc = getCenter(i) - ray.origin();
        bool bInside = oc.length() < radius[i];

        double tca = dot(oc, ray.direction());

        if (!bInside && (tca < 0)) {
            return false;
        }

        double thcSquare = pow(radius[i], 2) - oc.lengthSquared() + pow(tca, 2);
        if (thcSquare < 0) {
            return false;
        }

        double t = tca;
        t += bInside ? sqrt(thcSquare) : -sqrt(thcSquare);
        p = ray.origin() + ray.direction() * t;
        return true;
    };

    virtual Color getColor(int i, Point p, Vector3<double> v, const Light* light, bool inShadow) const override {
        // Vector3 normal = (p - Point(centerX, centerY, centerZ)) / radius;
        Vector3 normal = normalize((p - getCenter(i)) / radius[i]);
        // Vector3 normal = Point();
        return phongShading(i, normal, v, light, inShadow);
    }

};

#endif

#ifndef TRIANGLES_H
#define TRIANGLES_H

struct Triangles : public Primitives {

    std::vector<double> p0x, p0y, p0z;
    std::vector<double> p1x, p1y, p1z;
    std::vector<double> p2x, p2y, p2z;
    
    std::vector<double> normalX, normalY, normalZ;

    std::vector<double> detriment;
    std::vector<bool> doubleSided;

    Triangles() : Primitives() {};


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

    void setTriangle(int i, Point p0, Point p1, Point p2, Vector3<double> normal, bool doubleSided_, float kDiffuse_, float kSpecular_, float kAmbient_, float kGloss_, float reflectiveness_, Color diffuseColor_, Color specularColor_) {
        p0x[i] = p0[0];
        p0y[i] = p0[1];
        p0z[i] = p0[2];
        p1x[i] = p0[0];
        p1y[i] = p0[1];
        p1z[i] = p0[2];
        p2x[i] = p0[0];
        p2y[i] = p0[1];
        p2z[i] = p0[2];

        Vector3<double> temp = normalize(normal);
        normalX[i] = temp[0];
        normalY[i] = temp[1];
        normalZ[i] = temp[2];
        detriment[i] = -dot(temp, p0);

        doubleSided[i] = doubleSided_;

        setPrimitive(i, kDiffuse_, kSpecular_, kAmbient_, kGloss_, reflectiveness_, diffuseColor_, specularColor_);
        pType[i] = PrimitiveType::triangle;
    };

    void initializeVectors(int size) {
        p0x = std::vector<double>(size);
        p0y = std::vector<double>(size);
        p0z = std::vector<double>(size);
        p1x = std::vector<double>(size);
        p1y = std::vector<double>(size);
        p1z = std::vector<double>(size);
        p2x = std::vector<double>(size);
        p2y = std::vector<double>(size);
        p2z = std::vector<double>(size);

        normalX = std::vector<double>(size);
        normalY = std::vector<double>(size);
        normalZ = std::vector<double>(size);
        detriment = std::vector<double>(size);

        doubleSided = std::vector<bool>(size);

        Primitives::initializeVectors(size);
    }

    void setNormal(int i, const Vector3<double> v) {
        Vector3<double> temp = normalize(v);
        normalX[i] = temp[0];
        normalY[i] = temp[1];
        normalZ[i] = temp[2];
        detriment[i] = -dot(temp, Point(p0x[i], p0y[i], p0z[i]));
    };

    bool intersetRay(int i, Point& p, const Ray ray) const override {
        Vector3<double> normal = getNormal(i);
        double vd = dot(normal, ray.direction());
        if (vd == 0) {
            return false;
        }
        if (!doubleSided[i] && vd > 0) {
            return false;
        } 

        double vo = -(dot(normal, ray.origin()) + detriment[i]);
        double t = vo / vd;
        
        if (t < 0) {
            return false;
        }
        
        p = ray.at(t);

        Point p0 = Point(p0x[i], p0y[i], p0z[i]);
        Point p1 = Point(p1x[i], p1y[i], p1z[i]);
        Point p2 = Point(p2x[i], p2y[i], p2z[i]);

        bool dot1 = dot(normal, cross(p1 - p0, p - p0)) > 0;
        bool dot2 = dot(normal, cross(p2 - p1, p - p1)) > 0;
        bool dot3 = dot(normal, cross(p0 - p2, p - p2)) > 0;
        bool intersect = dot1 && dot2 && dot3;

        return intersect;
    }

    inline Vector3<double> getNormal(int i) const { return Vector3<double>(normalX[i], normalY[i], normalZ[i]); }
    virtual Color getColor(int i, Point p, Vector3<double> v, const Light* light, bool inShadow) const override {
        return phongShading(i, getNormal(i), v, light, inShadow);
    }
    
};

#endif

