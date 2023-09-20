#include "utils/logger.h"
#include "systems/window/window_context.h"

#include "demos/task-one/task_one.h"
#include "resources/images/icon/icon.h"
#include "systems/rendering/perspective_camera.h"
#include "systems/settings/render_settings.h"

#include "pTween.h"

#define ICON_DATA { image_width, image_height, image_data }
#define WINDOW_PARAMS { 1600, 900, "Task 1", 22.0f, ICON_DATA }

// N.B always do camera update last
int main(void)
{
    Logger::SetPriority(Logger::LogPriority::Debug);
    
    WindowContext context = WindowContext(WINDOW_PARAMS);
    PerspectiveCamera camera = PerspectiveCamera(context);
    RenderSettings settings = RenderSettings(true);
    
    TaskOne demo = TaskOne(context, camera, settings);

    auto update = [&](float deltaTime)
    {
        pTween::pTweenStep(glfwGetTime());

        demo.OnGameUpdate(deltaTime);
        demo.OnGUIUpdate();
        camera.HandleKeyInput(deltaTime);
        camera.OnUpdate(deltaTime);
    };

    context.SetDeltaUpdate(update);
    context.BeginLoop();

    Logger::Log("Exiting...");
    return 0;
}