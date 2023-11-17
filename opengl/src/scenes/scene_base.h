#pragma once

// local systems
#include "../systems/rendering/perspective_camera.h"
#include "../systems/settings/render_settings.h"
#include "../systems/window/window_context.h"
#include "../systems/shader/shader_component.h"
#include "../systems/settings/render_settings.h"
// lights
#include "../systems/rendering/scene_lighting_data.h"
// entities
#include "../systems/rendering/transform.h"
#include "../systems/rendering/perspective_camera.h"
#include "../resources/fonts/forkawesome-icons.h"

// drawables
#include "../systems/drawable/impl/cone.h"
#include "../systems/drawable/impl/cone.h"
#include "../systems/drawable/impl/cone.h"
#include "../systems/drawable/cubemap.h"
#include "../systems/drawable/billboard.h"

// chess pieces
#include "../utils/pieces/impl/pawn.h"
#include "../utils/pieces/impl/king.h"
#include "../utils/pieces/impl/queen.h"
#include "../utils/pieces/impl/knight.h"
#include "../utils/pieces/impl/bishop.h"
#include "../utils/pieces/impl/rook.h"
#include "../systems/drawable/terrain.h"
#include "../utils/gui.h"

#include "scene_hierarchy.h"

// local utils
#include "pTween.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <functional>
#include <vector>
#include <filesystem>

struct CameraSnapshot
{
    CameraSettings settings;
    glm::vec3 Position;
    bool hasBeenSaved;
};

class SceneBase {
protected:
    PerspectiveCamera& m_camera;
    GLFWwindow* m_window;
    RenderSettings& m_renderSettings;
    
public:
    SceneHierarchy Hierarchy;
    SceneLightingData LightingData;
    glm::vec4 ScreenClearColor = glm::vec4(0.18f, 0.18f, 0.18f, 1.0f);
    virtual ~SceneBase() = default;
    SceneBase(WindowContext& context, PerspectiveCamera& camera, RenderSettings& settings)
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
    virtual void OnExit() = 0;

    CameraSnapshot CamSnapshot;

    void SaveCamSnapshot()
    {
        CamSnapshot = {
            m_camera.Settings, m_camera.Transform.Position
        };
    }

    CameraSnapshot GetCameraSnapshot()
    {
        return CamSnapshot = {
            m_camera.Settings, m_camera.Transform.Position
        };
    }

    void LoadCamSnapShot()
    {
        if (!CamSnapshot.hasBeenSaved) return;
        
        m_camera.Settings = CamSnapshot.settings;
        m_camera.Transform.Position = CamSnapshot.Position;
    }
};

