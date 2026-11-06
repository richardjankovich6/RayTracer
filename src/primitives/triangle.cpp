#include "triangle.h"
#include "ray.h"

void Triangle::findD() {
    Vector3<double> v1 = p3 - p2;
    Vector3<double> v2 = p1 - p2;

    normal = unitVec(cross(v1, v2));

    // this is a fucking thing
    d = -dot(normal, p1);
};

void Triangle::setNormal(Vector3<double> v) {
    normal = normalize(v);
    // this is a fucking thing
    d = -dot(normal, p1);
};

bool Triangle::intersetRay(Point& p, const Ray ray) const {
    double vd = dot(normal, ray.direction());
    if (vd == 0) {
        return false;
    }
    if (!doubleSided && vd > 0) {
        return false;
    } 

    double vo = -(dot(normal, ray.origin()) + d);
    double t = vo / vd;
    
    if (t < 0) {
        return false;
    }
    
    p = ray.at(t);

    bool dot1 = dot(normal, cross(p2 - p1, p - p1)) > 0;
    bool dot2 = dot(normal, cross(p3 - p2, p - p2)) > 0;
    bool dot3 = dot(normal, cross(p1 - p3, p - p3)) > 0;
    bool intersect = dot1 && dot2 && dot3;

    return intersect;
}