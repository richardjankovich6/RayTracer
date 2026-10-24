#include "sphere.h"

bool Sphere::intersetRay(Point& p, const Ray ray) const {
    Vector3 oc = center - ray.origin();
    bool bInside = oc.length() < radius;

    double tca = dot(oc, ray.direction());

    if (!bInside && (tca < 0)) {
        // ray points away from sphere
        return false;
    }

    double thcSquare = pow(radius, 2) - oc.lengthSquared() + pow(tca, 2);
    if (thcSquare < 0) {
        return false;
    }

    double t = tca;
    t += bInside ? sqrt(thcSquare) : -sqrt(thcSquare);
    p = ray.origin() + ray.direction() * t;
    return true;
}