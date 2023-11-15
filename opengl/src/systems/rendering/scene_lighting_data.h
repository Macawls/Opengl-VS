#pragma once
#include "lights/directional.h"
#include "lights/point.h"
#include "lights/spot.h"

class SceneLightingData
{
public:
    DirectionalLight Directional;
    std::vector<PointLight> PointLights;
};
