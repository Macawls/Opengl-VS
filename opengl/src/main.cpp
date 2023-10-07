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



const GLFWimage icon = { image_width, image_height, image_data };
const WindowParameters windowParams = { 1600, 900, "Task 1", 22.0f, icon };

// N.B Do camera update after rendering the scene
int main(void)
{
    Logger::SetPriority(Logger::LogPriority::Debug);
    
    WindowContext context = WindowContext(windowParams);
    PerspectiveCamera camera = PerspectiveCamera(context);
    RenderSettings settings = RenderSettings(context);

    DemoSelection demos = DemoSelection()
        .AddDemo("Task 1",  new TaskOne(context, camera, settings))
        .AddDemo("Cube",    new CubeDemo(context, camera, settings))
        .Init();


    auto applicationUpdate = [&](float deltaTime)
    {
        demos.Current->OnUpdate(deltaTime);
        
        ShowDemoWindow(demos.CurrentTitle, [&]() { 
            demos.Current->OnGUI(); 
        });

        camera.HandleKeyInput(deltaTime);
        camera.OnUpdate(deltaTime);

        demos.ShowSelectionWindow();
        settings.ShowRenderingWindow();

        
        pTween::pTweenStep(glfwGetTime());
    };

    context.SetDeltaUpdate(applicationUpdate);
    context.BeginLoop();
    
    Logger::Log("Exiting...");
    return 0;
}