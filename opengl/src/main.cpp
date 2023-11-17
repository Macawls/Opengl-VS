#include <unordered_map>
#include "utils/logger.h"

// demos
#include "scenes/scene_selection.h"
#include "scenes/chess-pieces/chess_pieces.h"
#include "scenes/cube/cube_test.h"
#include "scenes/task-one/task_one.h"
#include "scenes/task-two/task_two.h"

// systems
#include "main_menu_bar.h"
#include "pTween.h"
#include "resources/images/icon/icon.h"
#include "scenes/lighting/lighting_test.h"
#include "systems/rendering/perspective_camera.h"
#include "systems/settings/render_settings.h"
#include "systems/window/window_context.h"
#include "utils/gui.h"

const GLFWimage ICON = { image_width, image_height, image_data };
const WindowParameters WINDOW_PARAMS = { 1600, 900, "opengl", 22.0f, ICON };

static bool UI_ENABLED = true;


// N.B Do camera update after rendering the scene
int main()
{
    Logger::SetPriority(Logger::LogPriority::Debug);

    auto context = WindowContext(WINDOW_PARAMS);
    auto camera = PerspectiveCamera(context);
    auto settings = RenderSettings(context);

    SceneSelection demos = SceneSelection()
    .AddScene("Lighting Test", new LightingTestScene(context, camera, settings))
    //.AddScene("Task Three",     new TaskThree(context, camera, settings))
    .AddScene("Task Two",       new TaskTwo(context, camera, settings))
    .AddScene("Chess Pieces",   new ChessPiecesScene(context, camera, settings))
    .AddScene("Task One",       new TaskOne(context, camera, settings))
    .AddScene("Cube Test",      new CubeTestScene(context, camera, settings))
    .Init();

    auto menuBar = MainMenuBar(4.0f, settings, demos);

    auto appUpdate = [&](const float deltaTime)
    {
        const float coefficient = settings.Config.TimeScale;
        const float delta = deltaTime * coefficient;
        
        demos.Current->OnUpdate(delta);
        camera.HandleKeyInput(delta);
        camera.HandleMouseInput(delta);
        camera.OnUpdate(delta);

        menuBar.Show(UI_ENABLED);

        if (UI_ENABLED)
        {
            ShowDemoGUI(demos.CurrentTitle,[&]
            {
                demos.Current->OnGui();
            });
        }
        
        pTween::pTweenStep(glfwGetTime() * coefficient);
    };

    context.SetDeltaUpdate(appUpdate);
    context.BeginLoop();
    
    Logger::Log("Exiting...");
    return 0;
}