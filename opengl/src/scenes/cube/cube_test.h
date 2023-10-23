#pragma once
#include "..\scene_base.h"

// https://github.com/c2d7fa/opengl-cube/blob/master/main.c


class CubeTestScene : virtual public SceneBase
{
public:
    using SceneBase::SceneBase;
    CubeTestScene(WindowContext& context, PerspectiveCamera& camera, RenderSettings& settings);
    
    void OnSetup() override;
    void OnUpdate(float deltaTime) override;
    void OnGui() override;
private:

    const TransformComponent StartingTransform = TransformComponent(
        glm::vec3(0.0f),
        glm::vec3(25.0f, 45.0f, 0.0f),
    glm::vec3(1.0f));

    TransformComponent m_cubeTransform;
    
    float vertices[24] = {
        // Front face
        0.5f,  0.5f,  0.5f,
       -0.5f,  0.5f,  0.5f,
       -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,

        // Back face
        0.5f,  0.5f, -0.5f,
       -0.5f,  0.5f, -0.5f,
       -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
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
        1.0f, 0.4f, 0.6f,
        1.0f, 0.9f, 0.2f,
        0.7f, 0.3f, 0.8f,
        0.5f, 0.3f, 1.0f,

        0.2f, 0.6f, 1.0f,
        0.6f, 1.0f, 0.4f,
        0.6f, 0.8f, 0.8f,
        0.4f, 0.8f, 0.8f,
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
};
