#include "utils/logger.h"
#include <unordered_map>

// demos
#include "demos/demo-selection.h"
#include "demos/task-one/task_one.h"
#include "demos/cube/cube.h"

// systems
#include "resources/images/icon/icon.h"
#include "systems/window/window_context.h"
#include "systems/rendering/perspective_camera.h"
#include "utils/gui.h"
#include "systems/settings/render_settings.h"
#include "pTween.h"

const GLFWimage ICON = { image_width, image_height, image_data };
const WindowParameters WINDOW_PARAMS = { 1600, 900, "opengl", 22.0f, ICON };

// N.B Do camera update after rendering the scene
int main()
{
    Logger::SetPriority(Logger::LogPriority::Debug);

    auto context = WindowContext(WINDOW_PARAMS);

    auto camera = PerspectiveCamera(context);
    auto settings = RenderSettings(context);

    DemoSelection demos = DemoSelection()
        .AddDemo("Task 1",  new TaskOne(context, camera, settings))
        .AddDemo("Cube",    new CubeDemo(context, camera, settings))
        .Init();

    auto appUpdate = [&](const float deltaTime)
    {
        demos.Current->OnUpdate(deltaTime);
        
        ShowDemoWindow(demos.CurrentTitle, [&]
        { 
            demos.Current->OnGui(); 
        });

        camera.HandleKeyInput(deltaTime);
        camera.OnUpdate(deltaTime);

        demos.ShowSelectionWindow();
        settings.ShowRenderingWindow();

        
        pTween::pTweenStep(glfwGetTime());
    };

    context.SetDeltaUpdate(appUpdate);
    context.BeginLoop();
    
    Logger::Log("Exiting...");
    return 0;
}