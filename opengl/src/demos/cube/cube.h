#pragma once
#include "../demo-base.h"

// https://github.com/c2d7fa/opengl-cube/blob/master/main.c


class CubeDemo : virtual public DemoBase
{
public:
    using DemoBase::DemoBase;
    CubeDemo(WindowContext& context, PerspectiveCamera& camera, RenderSettings& settings);

    glm::vec4 clearColour = glm::vec4(0.18f, 0.18f, 0.18f, 1.0f);

    void OnSetup() override;
    void OnUpdate(float deltaTime) override;
    void OnGUI() override;
private:

    float vertices[24] = {
        // Front face
        0.5,  0.5,  0.5,
       -0.5,  0.5,  0.5,
       -0.5, -0.5,  0.5,
        0.5, -0.5,  0.5,

        // Back face
        0.5,  0.5, -0.5,
       -0.5,  0.5, -0.5,
       -0.5, -0.5, -0.5,
        0.5, -0.5, -0.5,
    };
    
    
    unsigned int indices[36] = {
        // Front
        0, 1, 2,
        2, 3, 0,

        // Right
        0, 3, 7,
        7, 4, 0,

        // Bottom
        2, 6, 7,
        7, 3, 2,

        // Left
        1, 5, 6,
        6, 2, 1,

        // Back
        4, 7, 6,
        6, 5, 4,

        // Top
        5, 1, 0,
        0, 4, 5,
    };

    float colors[24] = {
        1.0, 0.4, 0.6,
        1.0, 0.9, 0.2,
        0.7, 0.3, 0.8,
        0.5, 0.3, 1.0,

        0.2, 0.6, 1.0,
        0.6, 1.0, 0.4,
        0.6, 0.8, 0.8,
        0.4, 0.8, 0.8,
    };

    const char *cubeVert =
#include "../../resources/shaders/cube/cube_shader.vert"
;
    const char *cubeFrag =
#include "../../resources/shaders/cube/cube_shader.frag"
;
    // Cube
    unsigned int cubeVBO, cubeVAO, cubeEBO;

    Shader cubeShader = Shader()
                        .SetVertexSource(cubeVert)
                        .SetFragmentSource(cubeFrag)
                        .Compile();
    // Cube Variables
    glm::vec4 cubeColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    TransformComponent cubeTransform = TransformComponent();
};
