#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <functional>

// local
#include "../../systems/window/window_context.h"
#include "../../systems/shader/shader.h"
#include "../../systems/settings/render_settings.h"
#include "../../systems/rendering/transform.h"
#include "../../systems/rendering/perspective_camera.h"

// https://github.com/c2d7fa/opengl-cube/blob/master/main.c

constexpr glm::vec3 StartingRotation = glm::vec3(25.0f, 45.0f, 0.0f);
//constexpr glm::vec3 StartingRotation = glm::vec3(0.0f, 0.0f, 0.0f);

class CubeDemo
{
public:
    CubeDemo(WindowContext& context, PerspectiveCamera& camera, RenderSettings& settings);

    glm::vec4 clearColour = glm::vec4(0.18f, 0.18f, 0.18f, 1.0f);

    std::function<void()> OnGUIUpdate;
    std::function<void(float deltaTime)> OnGameUpdate;
private:

    GLFWwindow* m_window;
    PerspectiveCamera& m_camera;
    RenderSettings& m_renderSettings;


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
    TransformComponent Transform = TransformComponent();

    
    void InitCube();
};
