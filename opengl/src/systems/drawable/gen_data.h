#pragma once
#include "glm/ext/scalar_constants.hpp"
static constexpr float PI = glm::pi<float>();

// Stacks ->    Vertical divisions
// Sectors ->   Horizontal divisions

//              Sectors Stacks
// Hi Def ->    24      12
// Mid Def ->   18      9
// Low Def ->   12      6
struct SphereGenData
{
    float Radius = 0.5f;
    int Sectors = 12;
    int Stacks = 6;
};
static SphereGenData SPHERE_GEN = SphereGenData();

//              Sectors Stacks 
// Hi Def ->    36      8
// Mid Def ->   18      4
// Low Def ->   9       2
// Note -> For cone shape, top is 2.0f bottom is 0.0f, won't work another way rn, needs fixing but for now its fine
// normal 0.5, 0.5
struct CylinderGenData
{
    const float BottomRadius = 0.5f;
    const float TopRadius = 0.5f;
    const float Height = 1.0f;
    const int Sectors = 9;
    const int Stacks = 2;
};
static CylinderGenData CYLINDER_GEN = CylinderGenData();
static CylinderGenData CONE_GEN = CylinderGenData { 0.0f,2.0f, 1.0f,18, 4 };