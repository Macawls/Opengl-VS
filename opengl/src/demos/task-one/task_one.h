#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <functional>
#include <vector>

// local
#include "../../systems/window/window_context.h"
#include "../../systems/shader/shader.h"
#include "../../systems/settings/render_settings.h"
#include "../../systems/rendering/transform.h"
#include "../../systems/rendering/perspective_camera.h"
#include "../../systems/rendering/primitives/cube.h"
#include "../../systems/rendering/terrain.h"

class TaskOne
{
public:
    TaskOne(WindowContext& context, PerspectiveCamera& camera, RenderSettings& settings);
    ~TaskOne()
    { 
        m_chessBorder = nullptr;
        m_terrain = nullptr;
    }

    std::function<void()> OnGUIUpdate;
    std::function<void(float deltaTime)> OnGameUpdate;

private:
    const char* m_cubeVertSource =
#include "../../resources/shaders/basic.vert"
;
    const char* m_cubeFragSource =
#include "../../resources/shaders/basic.frag"
;
    const char* m_terrainVertSource =
#include "../../resources/shaders/terrain.vert"
;
    const char* m_terrainFragSource =
#include "../../resources/shaders/terrain.frag"
;

    
    WindowContext& m_context;
    GLFWwindow* m_window;
    PerspectiveCamera& m_camera;
    RenderSettings& m_renderSettings;

    glm::vec4 m_clearColor = glm::vec4(0.18f, 0.18f, 0.18f, 1.0f);

    float m_cellMaxOffset = 0.5f;
    Cube* m_chessBorder = nullptr;
    std::vector<Cube> m_cells;

    Terrain* m_terrain = nullptr;

    
    bool m_cameraUnlocked = false;
    int m_currentCamIndex = 0;
    TransformComponent m_newCamPosition;

    const float m_camDistance = 9.0f;
    const TransformComponent m_cameraPositions[3] = {
        
        TransformComponent(glm::vec3(m_camDistance, m_camDistance, m_camDistance), glm::vec3(38.0f, -45.0f, 0.0f), glm::vec3(1.0f)),
        
        TransformComponent(glm::vec3(0.0f, m_camDistance, m_camDistance), glm::vec3(45.0f, 0.0f, 0.0f), glm::vec3(1.0f)),
        
        TransformComponent(glm::vec3(0.0f, 0.0f, m_camDistance), glm::vec3(0.0f), glm::vec3(1.0f))
	};

    std::vector<Cube> generate_chess_board();
    
    void render_ui();
    void next_cam_pos();
    void prev_cam_pos();
    void check_new_cam_pos();
    void handle_input();
};


