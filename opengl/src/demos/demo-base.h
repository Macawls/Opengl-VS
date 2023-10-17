#pragma once

// local systems
#include "../systems/rendering/perspective_camera.h"
#include "../systems/settings/render_settings.h"
#include "../systems/window/window_context.h"
#include "../systems/shader/shader.h"
#include "../systems/settings/render_settings.h"
#include "../systems/rendering/transform.h"
#include "../systems/rendering/perspective_camera.h"
#include "../systems/rendering/primitives/cube.h"
#include "../systems/rendering/terrain.h"
#include "../utils/gui.h"

// local utils
#include "pTween.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <functional>
#include <vector>

class DemoBase {
protected:
    PerspectiveCamera& m_camera;
    GLFWwindow* m_window;
    RenderSettings& m_renderSettings;
    glm::vec4 m_clearColour = glm::vec4(0.18f, 0.18f, 0.18f, 1.0f);

public:
    virtual ~DemoBase() = default;
    DemoBase(WindowContext& context, PerspectiveCamera& camera, RenderSettings& settings)
        : m_camera(camera),
        m_window(context.GetGlfwWindow()),
        m_renderSettings(settings),
        Context(context) 
    { 
        
    }
    
    WindowContext& Context;

    virtual void OnSetup() = 0;
    virtual void OnUpdate(float deltaTime) = 0;
    virtual void OnGui() = 0;
};

