#include "cube.h"

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

constexpr glm::vec3 StartingRotation = glm::vec3(25.0f, 45.0f, 0.0f);

CubeDemo::CubeDemo(WindowContext& context, PerspectiveCamera& camera, RenderSettings& settings)
    : DemoBase(context, camera, settings)
{
    Transform.SetRotation(StartingRotation);
    InitCube();
}

void CubeDemo::OnSetup()
{
    m_camera.Reset();
    m_camera.Transform.SetPosition(CAMERA_STARTING_POSITION);
}

void CubeDemo::OnUpdate(float deltaTime)
{
    glClearColor(clearColour.r, clearColour.g, clearColour.b, clearColour.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = m_camera.GetProjectionMatrix();
    glm::mat4 view = m_camera.GetViewMatrix();

    // Calculate the MVP matrix
    glm::mat4 mvp = projection * view * Transform.GetModelMatrix();

    cubeShader.Use().SetMat4("mvp", mvp);

    // Draw cube
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void CubeDemo::OnGUI()
{
    ImGui::SetNextWindowPos(ImVec2(Context.ImGUI.IO->DisplaySize.x - 490, 20));
    ImGui::SetNextWindowSize(ImVec2(470, 0));

    ImGui::Begin("Rendering");
    m_renderSettings.ShowInfo();
    NormalizedMousePosition* pos = Context.GetNormalizedMousePosition();
    ImGui::Spacing();
    ImGui::Text("Cursor Pos:     %.3f, %.3f", pos->x, pos->y);
    ImGui::Separator();
    ImGui::Text("Settings");
    m_renderSettings.ShowImGuiTabBar();
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(20, 20));
    ImGui::SetNextWindowSize(ImVec2(550, 0), ImGuiCond_Always);

    ImGui::Begin("Cube Demo");
    if (ImGui::BeginTabBar("Tabs"))
    {
        if (ImGui::BeginTabItem("Parameters"))
        {
            if (ImGui::BeginTabBar("Control Tabs"))
            {
                if (ImGui::BeginTabItem("Cube"))
                {
                    ImGui::SliderFloat3("Position", glm::value_ptr(Transform.Position), -5.0f, 5.0f);
                    ImGui::SliderFloat3("Rotation", glm::value_ptr(Transform.Rotation), -180.0f, 180.0f);
                    ImGui::SliderFloat3("Scale", glm::value_ptr(Transform.Scale), 0.1f, 10.0f);

                    if (ImGui::Button("Reset"))
                    {
                        Transform.Reset().SetRotation(StartingRotation);
                    }

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
                    ImGui::ColorEdit4("Background", glm::value_ptr(clearColour));
                    ImGui::EndTabItem();

                    if (ImGui::Button("Reset"))
                    {
                        clearColour = glm::vec4(0.18f, 0.18f, 0.18f, 1.0f);
                    }
                }

                ImGui::EndTabBar();
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Logs"))
        {
            ImGui::BeginChild("LogScrollRegion", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
            for (const std::string& logEntry : Logger::GetHistory())
            {
                ImGui::TextUnformatted(logEntry.c_str());
            }
            ImGui::EndChild();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void CubeDemo::InitCube()
{
    // VAO
    GLCall(glGenVertexArrays(1, &cubeVAO));
    GLCall(glBindVertexArray(cubeVAO));

    // VBO
    GLCall(glGenBuffers(1, &cubeVBO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, cubeVBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW));
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors));

    // EBO
    GLCall(glGenBuffers(1, &cubeEBO));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    // Set up vertex attribute pointers for position and color
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0));
    GLCall(glEnableVertexAttribArray(0));

    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)sizeof(vertices)));
    GLCall(glEnableVertexAttribArray(1));

    // Unbind VBO and VAO
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
}


