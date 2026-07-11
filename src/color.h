#pragma once

#include <ostream>
#include "vector3.h"
#include "globals.h"

using color = Vector3<int>;
inline std::ostream& operator<<(std::ostream& out, const color& v) {
    return out << v.vec[0] << ", " << v.vec[1] << ", " << v.vec[2] << "  ";
}

// TODO: remove test function
color getColorDontUse(int w, int h) {

    float r = static_cast<float>(h) / (height - 1);
    float g = static_cast<float>(w) / (width - 1);
    constexpr double b = 0.0;

    int ir = static_cast<int>(255.999 * r);
    int ig = static_cast<int>(255.999 * g);
    int ib = static_cast<int>(255.999 * b);

    return Vector3<int>(ir, ig, ib);
}