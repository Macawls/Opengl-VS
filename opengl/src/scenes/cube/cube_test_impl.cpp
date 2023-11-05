#include "cube_test.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "imgui/imgui.h"
#include "../../utils/logger.h"
#include "../../utils/gl_call.h"




CubeTestScene::CubeTestScene(WindowContext& context, PerspectiveCamera& camera, RenderSettings& settings)
    : SceneBase(context, camera, settings)
{
    m_cubeTransform.Copy(StartingTransform);
    
    // VAO
    GLCall(glGenVertexArrays(1, &cubeVAO));
    GLCall(glBindVertexArray(cubeVAO));

    // VBO
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices + sizeof colors, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof vertices, vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof vertices, sizeof colors, colors);

    // EBO
    glGenBuffers(1, &cubeEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set up vertex attribute pointers for position and color
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
    GLCall(glEnableVertexAttribArray(0));

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)sizeof vertices);
    glEnableVertexAttribArray(1);

    // Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void CubeTestScene::OnSetup()
{
    m_camera.Reset();
    m_camera.Transform.SetPosition(CAMERA_STARTING_POSITION);
}

void CubeTestScene::OnUpdate(float deltaTime)
{
    glClearColor(ScreenClearColor.r, ScreenClearColor.g, ScreenClearColor.b, ScreenClearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const glm::mat4 projection = m_camera.GetProjectionMatrix();
    const glm::mat4 view = m_camera.GetViewMatrix();

    m_cubeTransform.Rotation.y += 20.0f * deltaTime;

    // Calculate the MVP matrix
    const glm::mat4 mvp = projection * view * m_cubeTransform.GetModelMatrix();

    cubeShader.Use().SetMat4("mvp", mvp);

    // Draw cube
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void CubeTestScene::OnGui()
{
    if (ImGui::BeginTabBar("Control Tabs"))
    {
        if (ImGui::BeginTabItem("Cube"))
        {
            m_cubeTransform.ShowControls(StartingTransform.Position, StartingTransform.Rotation);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Camera"))
        {
            ImGui::SliderFloat3("Position", glm::value_ptr(m_camera.Transform.Position), -3.0f, 15.0f);
            ImGui::SliderFloat3("Rotation##Cam", glm::value_ptr(m_camera.Transform.Rotation), -180.0f, 180.0f);

            ImGui::SliderFloat("FOV", &m_camera.Settings.Fov, 5.0f, 140.0f);
            ImGui::SliderFloat("Speed", &m_camera.Settings.Speed, 2.0f, 10.0f);

            if (ImGui::Button("Reset"))
            {
                m_camera.Transform
                    .Reset()
                    .SetPosition(CAMERA_STARTING_POSITION);

                m_camera.Settings = CameraSettings();
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Background"))
        {
            ImGui::ColorEdit4("Background", glm::value_ptr(ScreenClearColor));
            ImGui::EndTabItem();

            if (ImGui::Button("Reset"))
            {
                ScreenClearColor = glm::vec4(0.18f, 0.18f, 0.18f, 1.0f);
            }
        }

        ImGui::EndTabBar();
    }
}


