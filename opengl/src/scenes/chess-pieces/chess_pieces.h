#pragma once
#include "../scene_base.h"
#include "../../utils/Pieces/impl/pawn.h"

class ChessPiecesScene : virtual public SceneBase {
public:
    using SceneBase::SceneBase;

    ChessPiecesScene(WindowContext& context, PerspectiveCamera& camera, RenderSettings& settings);
    
    ~ChessPiecesScene() override
    {
        
    }
    
    void OnSetup() override;
    void OnUpdate(float deltaTime) override;
    void OnGui() override;
    void OnExit() override;

private:
    inline static const char* m_basicVertTex =
#include "../resources/shaders/basicTex.vert"
        ;
    inline static const char* m_basicFragTex =
    #include "../resources/shaders/basicTex.frag"
        ;

    inline static const char* texPath = "src/resources/textures/rock.png";
};




