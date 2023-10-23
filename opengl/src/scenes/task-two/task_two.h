#pragma once
#include "../scene_base.h"




class TaskTwo : virtual public SceneBase {
public:
    using SceneBase::SceneBase; // inherit constructor
    
    TaskTwo(WindowContext& context, PerspectiveCamera& camera, RenderSettings& settings);

    ~TaskTwo() override
    { 
        m_chessBorder = nullptr;
        m_terrain = nullptr;
    }
    
    void OnSetup() override;
    void OnUpdate(float deltaTime) override;
    void OnGui() override;

private:
    

    
    // Textures
    inline static const char* m_basicVert =
#include "../../resources/shaders/basic.vert"
;
    inline static const char* m_basicFrag =
#include "../../resources/shaders/basic.frag"
;
    inline static const char* m_basicVertTex =
#include "../../resources/shaders/basicTex.vert"
;
    inline static const char* m_basicFragTex =
#include "../../resources/shaders/basicTex.frag"
;
    const char* m_terrainVertSource =
#include "../../resources/shaders/terrain.vert"
;
    const char* m_terrainFragSource =
#include "../../resources/shaders/terrain.frag"
;
    const char* m_terrainVertSourceTex =
#include "../../resources/shaders/terrainTex.vert"
;
    const char* m_terrainFragSourceTex =
#include "../../resources/shaders/terrainTex.frag"
;

    // textures
    const char* pieceTexturePath = "src/resources/textures/ground.png";
    const char* borderTexturePath = "src/resources/textures/metal.png";
    const char* m_terrainHeightPath = "src/resources/images/terrain/berg-river.png";
    const char* m_terrainTexturePath = "src/resources/textures/rock.png";
    
    inline static constexpr TerrainOptions terrain_params = {
            200, // width
            200, // height
            30.0f, // max height
        { 10.0f, 10.0f } // repeat factor
    };

    float m_cellMaxOffset = 0.5f;
    Drawable* m_chessBorder = nullptr;
    std::vector<Drawable*> m_chessCells;

    Terrain* m_terrain = nullptr;
    pTweenObject m_terrainTween;
    
    bool m_cameraUnlocked = false;
    int m_currentCamIndex = 0;
    TransformComponent m_newCamPosition;
   
    const float m_camDistance = 9.0f;
    const TransformComponent m_cameraPositions[3] = {

        TransformComponent(
            glm::vec3(m_camDistance, m_camDistance, m_camDistance),
            glm::vec3(38.0f, -45.0f, 0.0f),
            glm::vec3(1.0f)),
        
        TransformComponent(
            glm::vec3(0.0f, m_camDistance, m_camDistance),
            glm::vec3(45.0f, 0.0f, 0.0f),
            glm::vec3(1.0f)),
        
        TransformComponent(
            glm::vec3(0.0f, m_camDistance, 0.0f),
            glm::vec3(90.0f, 0.0f, 0.0f),
            glm::vec3(1.0f))
	};

    std::vector<Drawable*> generate_chess_board(const char* pieceTexturePath, const glm::vec3& darkPieceColor, const glm::vec3& lightPieceColor) const;
    
    void render_ui();
    void next_cam_pos();
    void prev_cam_pos();
    void check_new_cam_pos();
    void handle_input();
    void play_cell_anim();
};


