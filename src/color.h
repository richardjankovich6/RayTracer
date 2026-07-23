#pragma once

#include <ostream>
#include "vector3.h"

#ifndef COLOR_H
#define COLOR_H

inline int clamp(int i, int min = 0, int max = 255) {
    return i < min ? min : i > max ? max : i;
}

using Color = Vector3<int>;
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