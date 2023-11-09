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
    glm::vec3 LightColor = glm::vec3(1.0f);

    const char *svertSrc =
#include "../../resources/shaders/basicTexLight.vert"
;
    const char *sfragSrc =
#include "../../resources/shaders/basicTexLight.frag"
;

    const char *cvertSrc =
#include "../../resources/shaders/basic.vert"
;
    
    const char *cfragSrc =
#include "../../resources/shaders/basic.frag"
;
    
    const char* texPath = "src/resources/textures/cobble.png";
    
    Sphere* subject;
    Drawable* light;
};
