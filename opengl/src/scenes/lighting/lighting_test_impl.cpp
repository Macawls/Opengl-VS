#include "lighting_test.h"

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

LightingTestScene::LightingTestScene(WindowContext& context, PerspectiveCamera& camera, RenderSettings& settings)
    : SceneBase(context, camera, settings)
{
    subject = new Sphere(ShaderComponent(svertSrc, sfragSrc),
        glm::vec3(1.0f),
        TextureComponent(texPath));

    light = new Sphere(ShaderComponent(cvertSrc, cfragSrc),
        glm::vec3(1.0f));

    SceneHierarchy.AddDrawable(subject).AddDrawable(light);
}

void LightingTestScene::OnSetup()
{
    light->Transform.SetPositionX(1.5f);
    light->Transform.SetScale(glm::vec3(0.3f));
    subject->Transform.GuiDisplay = "subject";
    light->Transform.GuiDisplay = "light";
    
    //sphere->Transform.SetRotation(glm::vec3(25.0f, 45.0f, 0.0f));
    m_camera.Reset();
    m_camera.Transform.SetPosition(CAMERA_STARTING_POSITION);
}

void LightingTestScene::OnUpdate(float deltaTime)
{
    glClearColor(ScreenClearColor.r, ScreenClearColor.g, ScreenClearColor.b, ScreenClearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   

    // Rendering
    subject->Shader.Use()
    .SetMat4("model", subject->Transform.GetModelMatrix())
    .SetMat4("view", m_camera.GetViewMatrix())
    .SetMat4("projection", m_camera.GetProjectionMatrix())
    .SetVec3("viewPos", m_camera.Transform.Position)
    .SetVec3("color", subject->Color)
    .SetVec3("lightPosition", light->Transform.Position)
    .SetVec3("lightColor", LightColor);
    
    
    subject->Texture.Bind(0);
    subject->Shader.SetInt("texture1", 0);
    
    // Draw 
    glBindVertexArray(subject->Vao);
    glDrawElements(GL_TRIANGLES, subject->Indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind
    glBindVertexArray(0);
    subject->Texture.Unbind();

    light->Draw(m_camera);

    constexpr auto up = glm::vec3(0.0f, 1.0f, 0.0f);
    light->Transform.RotateAround(subject->Transform.Position, up, 30, deltaTime);
}

void LightingTestScene::OnGui()
{
    if (ImGui::BeginTabBar("Control Tabs"))
    {
        if (ImGui::BeginTabItem("Lighting"))
        {
            ImGui::ColorPicker4("lightColor", value_ptr(LightColor));
            ImGui::EndTabItem();
        }

        
        if (ImGui::BeginTabItem("Cube"))
        {
            SceneHierarchy.ShowDrawablesTree();
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


