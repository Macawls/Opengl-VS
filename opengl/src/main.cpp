#include "utils/logger.h"
#include "systems/window/window_context.h"

#include "demos/cube/cube.h"
#include "resources/images/icon/icon.h"

#define ICON_DATA { image_width, image_height, image_data }
#define WINDOW_PARAMS { 1600, 650, "Cube Demo", 22.0f, ICON_DATA }


int main(void)
{
    Logger::SetPriority(Logger::LogPriority::Debug);
    WindowContext context = WindowContext(WINDOW_PARAMS);

    CubeDemo demo = CubeDemo(context);

    auto update = [&](float deltaTime)
    {
        demo.GameUpdate(deltaTime);
        demo.UiUpdate();
    };

    context.SetDeltaUpdate(update);
    context.BeginLoop();

    Logger::Log("Exiting...");
    return 0;
}