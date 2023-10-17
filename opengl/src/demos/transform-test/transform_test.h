#pragma once
#include "../demo-base.h"


static bool CAM_TWEEN_PLAYING = false;

class Transform_Test : virtual public DemoBase {
public:
    using DemoBase::DemoBase; // inherit constructor

    Transform_Test(WindowContext& context, PerspectiveCamera& camera, RenderSettings& settings);

    ~Transform_Test()
    {
        m_cubeParent = nullptr;
        m_cubeChild = nullptr;
    }

    void OnSetup() override;
    void OnUpdate(float deltaTime) override;
    void OnGui() override;

private:
    const char* m_cubeVertSource =
#include "../../resources/shaders/basic.vert"
        ;
    const char* m_cubeFragSource =
#include "../../resources/shaders/basic.frag"
        ;

    glm::vec4 m_clearColor = glm::vec4(0.18f, 0.18f, 0.18f, 1.0f);

    Cube* m_cubeParent = nullptr;
    Cube* m_cubeChild = nullptr;
};




