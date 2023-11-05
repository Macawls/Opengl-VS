#pragma once
#include "../scene_base.h"

class LightingTestScene : virtual public SceneBase
{
public:
    using SceneBase::SceneBase;
    LightingTestScene(WindowContext& context, PerspectiveCamera& camera, RenderSettings& settings);
    
    void OnSetup() override;
    void OnUpdate(float deltaTime) override;
    void OnGui() override;
private:

    const char *vert =
#include "../../resources/shaders/basicTex.vert"
;
    const char *frag =
#include "../../resources/shaders/basicTex.frag"
;
    const char* texPath = "src/resources/textures/metal.png";
    
    Cube* cube;
    Pawn* pawn;
};
