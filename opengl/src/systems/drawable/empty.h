﻿#pragma once

#include "./drawable.h"

class Empty : public Drawable
{
public:
    using Drawable::Drawable;
    Empty(const ShaderComponent& shader, const glm::vec3& color);
};

inline Empty::Empty(const ShaderComponent& shader, const glm::vec3& color): Drawable(shader, color)
{
    Transform.GuiDisplay = "Empty";
}