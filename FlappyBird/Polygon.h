#pragma once

#include "Math.h"

// ポリゴンデータの格納用（三角形）
struct Polygon
{
    Vector3 a;
    Vector3 b;
    Vector3 c;
    
    Vector3 offsetA;
    Vector3 offsetB;
    Vector3 offsetC;
    
    void ComputeWorldTransform(Matrix4 transform);
    
};


// BOXデータの格納（Boudingboxで使う）
struct Cube
{
    Vector3 min;
    Vector3 max;
    
};




