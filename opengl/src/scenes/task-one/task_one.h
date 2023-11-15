#pragma once
#include "../scene_base.h"


class TaskOne : virtual public SceneBase {
public:
    using SceneBase::SceneBase; // inherit constructor
    
    TaskOne(WindowContext& context, PerspectiveCamera& camera, RenderSettings& settings);

    ~TaskOne() override
    { 
        m_chessBorder = nullptr;
        m_terrain = nullptr;
    }
    
    void OnSetup() override;
    void OnUpdate(float deltaTime) override;
    void OnGui() override;
    void OnExit() override;

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

    inline static bool CAM_TWEEN_PLAYING = false;

    float m_cellMaxOffset = 0.5f;
    Cube* m_chessBorder = nullptr;
    std::vector<Cube> m_cells;

    Terrain* m_terrain = nullptr;
    pTweenObject m_terrainTween;

    
    bool m_cameraUnlocked = false;
    int m_currentCamIndex = 0;
    TransformComponent m_newCamPosition;

    pTweenObject m_cameraTweenPosX;
    pTweenObject m_cameraTweenPosY;
    pTweenObject m_cameraTweenPosZ;

    pTweenObject m_cameraTweenRotX;
    pTweenObject m_cameraTweenRotY;
    pTweenObject m_cameraTweenRotZ;

   
    const float m_camDistance = 9.0f;

    const TransformComponent m_cameraPositions[3] = {
        TransformComponent(glm::vec3(m_camDistance, m_camDistance, m_camDistance),
            glm::vec3(38.0f, -45.0f, 0.0f),
            glm::vec3(1.0f)),
        
        TransformComponent(glm::vec3(0.0f, m_camDistance, m_camDistance),
            glm::vec3(45.0f, 0.0f, 0.0f),
            glm::vec3(1.0f)),
        
        TransformComponent(glm::vec3(0.0f, m_camDistance, 0.0f),
            glm::vec3(90.0f, 0.0f, 0.0f),
            glm::vec3(1.0f))
	};

    std::vector<Cube> generate_chess_board();
    std::vector<pTweenObject> m_cellTweens = std::vector<pTweenObject>();
    
    void render_ui();
    void next_cam_pos();
    void prev_cam_pos();
    void check_new_cam_pos();
    void handle_input();
    void play_cell_anim();
};


