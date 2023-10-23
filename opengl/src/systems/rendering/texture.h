﻿#pragma once
#include <string>
#include <stb_image/stb_image.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../utils/logger.h"

class Texture {
public:
    Texture() = default;
    Texture(const char* path);
    // Bind the texture to a specific texture unit
    void Bind(unsigned int unit) const;

    // Unbind the texture
    void Unbind() const;

    // Get the width of the texture
    int GetWidth() const { return m_width; }

    // Get the height of the texture
    int GetHeight() const { return m_height; }

private:
    unsigned int m_id;
    int m_width;
    int m_height;
};
