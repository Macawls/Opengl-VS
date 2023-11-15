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

    m_lightingData.Directional = DirectionalLight{
        glm::vec3(-0.2f, -1.0f, -0.3f),
        glm::vec3(0.05f, 0.05f, 0.05f),
        glm::vec3(0.4f),
        glm::vec3(0.5f)
    };

    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };
    
    auto Light1 = PointLight{
        pointLightPositions[0],
        1.0f,
        0.09f,
        0.032f,
        glm::vec3(0.05f),
        glm::vec3(0.8f),
        glm::vec3(1.0f)
    };

    auto Light2 = PointLight{
        pointLightPositions[1],
        1.0f,
        0.09f,
        0.032f,
        glm::vec3(0.05f),
        glm::vec3(0.8f),
        glm::vec3(1.0f)
    };

    auto Light3 = PointLight{
        pointLightPositions[2],
        1.0f,
        0.09f,
        0.032f,
        glm::vec3(0.05f),
        glm::vec3(0.8f),
        glm::vec3(1.0f)
    };

    auto Light4 = PointLight{
        pointLightPositions[3],
        1.0f,
        0.09f,
        0.032f,
        glm::vec3(0.05f),
        glm::vec3(0.8f),
        glm::vec3(1.0f)
    };


    m_lightingData.PointLights.push_back(Light1);
    m_lightingData.PointLights.push_back(Light2);
    m_lightingData.PointLights.push_back(Light3);
    m_lightingData.PointLights.push_back(Light4);

    SceneHierarchy.AddDrawable(subject).AddDrawable(light);
}

void LightingTestScene::OnSetup()
{
    m_camera.Transform.Position = glm::vec3(0.0f, 0.0f, 3.0f);
    
    light->Transform.SetPositionX(1.5f);
    light->Transform.SetScale(glm::vec3(0.3f));
    subject->Transform.GuiDisplay = "subject";
    light->Transform.GuiDisplay = "light";
    
    //sphere->Transform.SetRotation(glm::vec3(25.0f, 45.0f, 0.0f));
    LoadCamSnapShot();
}

void LightingTestScene::OnExit()
{
    SaveCamSnapshot();
}

void LightingTestScene::OnUpdate(float deltaTime)
{
    glClearColor(ScreenClearColor.r, ScreenClearColor.g, ScreenClearColor.b, ScreenClearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rendering
    /*
    subject->Shader.Use()
    .SetMat4("model", subject->Transform.GetModelMatrix())
    .SetMat4("view", m_camera.GetViewMatrix())
    .SetMat4("projection", m_camera.GetProjectionMatrix())
    .SetVec3("viewPosition", m_camera.Transform.Position)
    .SetVec3("color", subject->Color)
    .SetVec3("lightPosition", light->Transform.Position)
    .SetVec3("lightColor", LightColor);
    */

    subject->Shader.Use()
    .SetMat4("model", subject->Transform.GetModelMatrix())
    .SetMat4("view", m_camera.GetViewMatrix())
    .SetMat4("projection", m_camera.GetProjectionMatrix())
    .SetVec3("viewPosition", m_camera.Transform.Position)
    .SetVec3("color", subject->Color)
    .SetVec3("directionalLight.direction", m_lightingData.Directional.Direction)
    .SetVec3("directionalLight.ambient", m_lightingData.Directional.Ambient)
    .SetVec3("directionalLight.diffuse", m_lightingData.Directional.Diffuse)
    .SetVec3("directionalLight.specular", m_lightingData.Directional.Specular);
    
    auto count = m_lightingData.PointLights.size();
    for (size_t i = 0; i < count; i++)
    {
        std::string index = std::to_string(i);
        subject->Shader
        .SetVec3("pointLights[" + index + "].position", m_lightingData.PointLights[i].Position)
        .SetVec3("pointLights[" + index + "].ambient", m_lightingData.PointLights[i].Ambient)
        .SetVec3("pointLights[" + index + "].diffuse", m_lightingData.PointLights[i].Diffuse)
        .SetVec3("pointLights[" + index + "].specular", m_lightingData.PointLights[i].Specular)
        .SetFloat("pointLights[" + index + "].constant", m_lightingData.PointLights[i].Constant)
        .SetFloat("pointLights[" + index + "].linear", m_lightingData.PointLights[i].Linear)
        .SetFloat("pointLights[" + index + "].quadratic", m_lightingData.PointLights[i].Quadratic);
    }
    
    
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
            //ImGui::ColorPicker4("lightColor", value_ptr(LightColor));
            ImGui::EndTabItem();
        }

        
        if (ImGui::BeginTabItem("Cube"))
        {
            SceneHierarchy.GuiShowDrawablesTree();
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Background"))
        {
            ImGui::ColorEdit4("Background", value_ptr(ScreenClearColor));
            ImGui::EndTabItem();

            if (ImGui::Button("Reset"))
            {
                ScreenClearColor = glm::vec4(0.18f, 0.18f, 0.18f, 1.0f);
            }
        }

        if (ImGui::BeginTabItem("Camera"))
        {
            m_camera.GuiShowControls();
            ImGui::EndTabItem();
        }
        

        ImGui::EndTabBar();
    }
}



