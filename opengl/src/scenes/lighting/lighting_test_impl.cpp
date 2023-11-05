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
    cube = new Cube(ShaderComponent(vert, frag), glm::vec3(1.0f, 1.0f, 1.0f), TextureComponent(texPath));
    pawn = new Pawn(ShaderComponent(vert, frag), glm::vec3(1.0f), TextureComponent(texPath));

    SceneHierarchy.AddDrawable(pawn).AddDrawable(cube);
}

void LightingTestScene::OnSetup()
{
    cube->Transform.SetRotation(glm::vec3(25.0f, 45.0f, 0.0f));
    pawn->Transform.SetPositionX(-2.0f);
    m_camera.Reset();
    m_camera.Transform.SetPosition(CAMERA_STARTING_POSITION);
}

void LightingTestScene::OnUpdate(float deltaTime)
{
    glClearColor(ScreenClearColor.r, ScreenClearColor.g, ScreenClearColor.b, ScreenClearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SceneHierarchy.DrawAll(m_camera);
}

void LightingTestScene::OnGui()
{
    if (ImGui::BeginTabBar("Control Tabs"))
    {
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


