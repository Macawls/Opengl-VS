#include "lighting_test.h"

#define GLEW_STATIC
#include <fstream>
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

static glm::vec3 rainbow_color(float multiplier, float intensity)
{
    float red = 0.5f * (1.0f + intensity * std::sin(glm::two_pi<float>() * multiplier));
    float green = 0.5f * (1.0f + intensity * std::sin(glm::two_pi<float>() * multiplier + glm::two_pi<float>() / 3.0f));
    float blue = 0.5f * (1.0f + intensity * std::sin(glm::two_pi<float>() * multiplier + 2.0f * glm::two_pi<float>() / 3.0f));

    return glm::vec3(red, green, blue);
}

LightingTestScene::LightingTestScene(WindowContext& context, PerspectiveCamera& camera, RenderSettings& settings)
    : SceneBase(context, camera, settings)
{
    subject = new Sphere(ShaderComponent(svertSrc, sfragSrc),
        glm::vec3(1.0f),
        TextureComponent(texPath));

    subjectCube = new Cube(ShaderComponent(svertSrc, sfragSrc),
    glm::vec3(1.0f),
    TextureComponent(texPathCube));

    
    light = new Sphere(ShaderComponent(cvertSrc, cfragSrc),
        glm::vec3(1.0f));

    const std::vector<std::string> faces
    {
        "src/resources/textures/skybox/right.png",
        "src/resources/textures/skybox/left.png",
        "src/resources/textures/skybox/top.png",
        "src/resources/textures/skybox/bottom.png",
        "src/resources/textures/skybox/front.png",
        "src/resources/textures/skybox/back.png",
    };

    map = new Cubemap(faces, ShaderComponent(mapVertSrc, mapFragSrc));

    setup_lights(LightingData);

    Hierarchy.AddDrawable(subject).AddDrawable(light).AddDrawable(subjectCube);
}

void LightingTestScene::setup_lights(SceneLightingData& data)
{
    data.Directional = DirectionalLight
    {
        glm::vec3(-0.2f, -1.0f, -0.3f),
        glm::vec3(0.05f, 0.05f, 0.05f),
        glm::vec3(0.4f),
        glm::vec3(0.5f)
    };

    data.Spot = SpotLight();

    const glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(1.5f,  0.0f, -3.0f)
    };

    auto Light1 = PointLight{
        TransformComponent(pointLightPositions[0]),
        1.0f,
        0.09f,
        0.032f,
        glm::vec3(0.05f),
        glm::vec3(0.8f),
        glm::vec3(1.0f)
    };

    auto Light2 = PointLight{
        TransformComponent(pointLightPositions[1]),
        1.0f,
        0.09f,
        0.032f,
        glm::vec3(0.05f),
        glm::vec3(0.8f),
        glm::vec3(1.0f)
    };

    auto Light3 = PointLight{
        TransformComponent(pointLightPositions[2]),
        1.0f,
        0.09f,
        0.032f,
        glm::vec3(0.05f),
        glm::vec3(0.8f),
        glm::vec3(1.0f)
    };

    auto Light4 = PointLight{
        TransformComponent(pointLightPositions[3]),
        1.0f,
        0.09f,
        0.032f,
        glm::vec3(0.05f),
        glm::vec3(0.8f),
        glm::vec3(1.0f)
    };
    
    data.PointLights.push_back(Light1);
    data.PointLights.push_back(Light2);
    data.PointLights.push_back(Light3);
    data.PointLights.push_back(Light4);
}

void LightingTestScene::OnSetup()
{
    m_camera.Transform.Position = glm::vec3(0.0f, 0.0f, 3.0f);

    subjectCube->Transform.SetPositionX(-1.5f);
    subject->Transform.SetPositionX(1.5f);
    
    light->Transform.SetPositionX(1.5f);
    light->Transform.SetScale(glm::vec3(0.3f));
    subject->Transform.GuiDisplay = "Subject Sphere";
    subjectCube->Transform.GuiDisplay = "Subject Cube";
    subjectCube->Transform.SetScale(glm::vec3(0.9f));
    light->Transform.GuiDisplay = "Light";
    
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

    // structured binding
    const auto [View, Projection, Position, Front] = m_camera.GetData();
    
    subject->Shader.Use()
    .SetMat4("model", subject->Transform.GetModelMatrix())
    .SetMat4("view", View)
    .SetMat4("projection", Projection)
    .SetVec3("viewPosition", Position)
    .SetVec3("color", subject->Color)
    .SetVec3("directionalLight.direction", LightingData.Directional.Direction)
    .SetVec3("directionalLight.ambient", LightingData.Directional.Ambient)
    .SetVec3("directionalLight.diffuse", LightingData.Directional.Diffuse)
    .SetVec3("directionalLight.specular", LightingData.Directional.Specular)
    .SetVec3("spotLight.position", Position)
    .SetVec3("spotLight.direction", Front)
    .SetVec3("spotLight.ambient", LightingData.Spot.Ambient)
    .SetVec3("spotLight.diffuse", LightingData.Spot.Diffuse)
    .SetVec3("spotLight.specular", LightingData.Spot.Specular)
    .SetFloat("spotLight.constant", LightingData.Spot.Constant)
    .SetFloat("spotLight.linear",  LightingData.Spot.Linear)
    .SetFloat("spotLight.quadratic", LightingData.Spot.Quadratic)
    .SetFloat("spotLight.cutOff", LightingData.Spot.CutOff)
    .SetFloat("spotLight.outerCutOff", LightingData.Spot.OuterCutOff); 


    for (size_t i = 0; i < LightingData.PointLights.size(); i++)
    {
        std::string index = std::to_string(i);
        subject->Shader
        .SetVec3("pointLights[" + index + "].position", LightingData.PointLights[i].Transform.Position)
        .SetVec3("pointLights[" + index + "].ambient", LightingData.PointLights[i].Ambient)
        .SetVec3("pointLights[" + index + "].diffuse", LightingData.PointLights[i].Diffuse)
        .SetVec3("pointLights[" + index + "].specular", LightingData.PointLights[i].Specular)
        .SetFloat("pointLights[" + index + "].constant", LightingData.PointLights[i].Constant)
        .SetFloat("pointLights[" + index + "].linear", LightingData.PointLights[i].Linear)
        .SetFloat("pointLights[" + index + "].quadratic", LightingData.PointLights[i].Quadratic);
    }

        
    subject->Texture.Bind(0);
    subject->Shader.SetInt("texture1", 0);

    // Draw 
    glBindVertexArray(subject->Vao);
    glDrawElements(GL_TRIANGLES, subject->Indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind
    glBindVertexArray(0);
    subject->Texture.Unbind();

    
    subjectCube->Shader.Use()
    .SetMat4("model", subjectCube->Transform.GetModelMatrix())
    .SetMat4("view", View)
    .SetMat4("projection", Projection)
    .SetVec3("viewPosition", m_camera.Transform.Position)
    .SetVec3("color", subjectCube->Color)
    .SetVec3("directionalLight.direction", LightingData.Directional.Direction)
    .SetVec3("directionalLight.ambient", LightingData.Directional.Ambient)
    .SetVec3("directionalLight.diffuse", LightingData.Directional.Diffuse)
    .SetVec3("directionalLight.specular", LightingData.Directional.Specular)
    .SetVec3("spotLight.position", Position)
    .SetVec3("spotLight.direction", Front)
    .SetVec3("spotLight.ambient", LightingData.Spot.Ambient)
    .SetVec3("spotLight.diffuse", LightingData.Spot.Diffuse)
    .SetVec3("spotLight.specular", LightingData.Spot.Specular)
    .SetFloat("spotLight.constant", LightingData.Spot.Constant)
    .SetFloat("spotLight.linear",  LightingData.Spot.Linear)
    .SetFloat("spotLight.quadratic", LightingData.Spot.Quadratic)
    .SetFloat("spotLight.cutOff", LightingData.Spot.CutOff)
    .SetFloat("spotLight.outerCutOff", LightingData.Spot.OuterCutOff);

    for (size_t i = 0; i < LightingData.PointLights.size(); i++)
    {
        std::string index = std::to_string(i);

        subjectCube->Shader
        .SetVec3("pointLights[" + index + "].position", LightingData.PointLights[i].Transform.Position)
        .SetVec3("pointLights[" + index + "].ambient", LightingData.PointLights[i].Ambient)
        .SetVec3("pointLights[" + index + "].diffuse", LightingData.PointLights[i].Diffuse)
        .SetVec3("pointLights[" + index + "].specular", LightingData.PointLights[i].Specular)
        .SetFloat("pointLights[" + index + "].constant", LightingData.PointLights[i].Constant)
        .SetFloat("pointLights[" + index + "].linear", LightingData.PointLights[i].Linear)
        .SetFloat("pointLights[" + index + "].quadratic", LightingData.PointLights[i].Quadratic);
        
    }
    


    subjectCube->Texture.Bind(0);
    subjectCube->Shader.SetInt("texture1", 0);



    glBindVertexArray(subjectCube->Vao);
    glDrawElements(GL_TRIANGLES, subjectCube->Indices.size(), GL_UNSIGNED_INT, 0);

    const auto billboardSize = glm::vec2(2.0f, 2.0f);
    const auto rightWorld = glm::vec3(View[0][0], View[1][0], View[2][0]);
    const auto upWorld = glm::vec3(View[0][1], View[1][1], View[2][1]);
    
    // draw map last always
    map->Draw(View, Projection);

    /**/

    
    //light->Draw(m_camera);

    constexpr auto up = glm::vec3(0.0f, 1.0f, 0.0f);
    auto center = glm::vec3(0.0f);
    //light->Transform.RotateAround(subject->Transform.Position, up, 30, deltaTime);
    const auto speed = 30.f;
    for (size_t i = 0; i < LightingData.PointLights.size(); i++)
    {
        const float offset = glfwGetTime() + (static_cast<float>(i) + 2) * 4.0f;
        LightingData.PointLights[i].Diffuse = rainbow_color(offset * 0.1f, 0.8);
        
        LightingData.PointLights[i].Transform
        .RotateAround(center, up, speed, deltaTime);
    }

    
    subjectCube->Transform.RotateAround(center, up, 20.0f, deltaTime);
    subject->Transform.RotateAround(center, up, 20.0f, deltaTime);
}

void LightingTestScene::OnGui()
{
    if (ImGui::BeginTabBar("Control Tabs"))
    {
        if (ImGui::BeginTabItem("Lighting"))
        {
            LightingData.GuiShowLightControls();
            ImGui::EndTabItem();
        }

        
        if (ImGui::BeginTabItem("Cube"))
        {
            Hierarchy.GuiShowDrawablesTree();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Camera"))
        {
            m_camera.GuiShowControls();
            ImGui::EndTabItem();
        }
        

        ImGui::EndTabBar();
    }
}



