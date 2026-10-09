#pragma once

#include <ostream>
#include "vector3.h"

#ifndef COLOR_H
#define COLOR_H

inline int clamp(double i, double min = 0, double max = 1.0) {
    return i < min ? min : i > max ? static_cast<int>(max * 255.0) : static_cast<int>(i * 255.0);
}

using Color = Vector3<double>;
inline std::ostream& operator<<(std::ostream& out, const Color& c) {
    return out << clamp(c.x()) << ", " << clamp(c.y()) << ", " << clamp(c.z()) << "  ";
}


// Color& operator*=(Color This, double t) {
//     // Color c;
    
//     This[0] *= t;
//     This[1] *= t;
//     This[2] *= t;
//     return This;
// }

#endif