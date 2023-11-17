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
    void OnExit() override;
private:
    void setup_lights(SceneLightingData& data);

    const char *svertSrc =
#include "../../resources/shaders/basicTexLight-MultipleLights.vert"
;
    const char *sfragSrc =
#include "../../resources/shaders/basicTexLight-MultipleLights.frag"
;

    const char *cvertSrc =
#include "../../resources/shaders/basic.vert"
;
    
    const char *cfragSrc =
#include "../../resources/shaders/basic.frag"
;

    const char *mapVertSrc =
#include "../../resources/shaders/cubemap.vert"
;
    
    const char *mapFragSrc =
#include "../../resources/shaders/cubemap.frag"
;

    const char* pointLightPath = "src/resources/textures/icons/lights/point.png";
    
    const char* texPath = "src/resources/textures/cobble.png";
    const char* texPathCube = "src/resources/textures/metal.png";
    
    Sphere* subject;
    Cube* subjectCube;
    Drawable* light;
    Cubemap* map;
    
};
