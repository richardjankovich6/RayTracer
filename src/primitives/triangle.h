#pragma once

#include "primitive.h"

#ifndef TRIANGLE_H
#define TRIANGLE_H

class Triangle : public Primitive {
    

public:

    Triangle() {};

    bool intersetRay(Point& p, const Ray ray) const override {
        return false;   
    }

    
};

#endif