#pragma once
#include "configs.h"

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "../resources/fonts/forkawesome-icons.h"

#include "../window/window_context.h"

////////////////////////////////////////////
//    Set Opengl Context before calling Apply()
//    Configuration Example:
//    settings
//      .PolygonMode({ GL_FRONT_AND_BACK, GL_FILL })
//      .Depth({ true, GL_LESS })
//      .Blending({ true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_FUNC_ADD })
//      .Culling({ true, GL_BACK, GL_CCW })
//      .Apply();
// 
////////////////////////////////////////////
class RenderSettings
{
public:
    inline static const char* SETTINGS_HEADER = ICON_FK_COGS " Settings";
    inline static const char* INFO_HEADER = ICON_FK_TELEVISION " Information";
    inline static const char* LOGS_HEADER = ICON_FK_BARS " Logs";
    inline static const char* CONFIG_HEADER = ICON_FK_WRENCH " Configuration";
    inline static const char* WINDOW_HEADER = "System";
    RenderSettings(const RenderConfig& config, WindowContext& context);
    RenderSettings(WindowContext& context);


    RenderSettings& PolygonMode(PolygonModeConfig config);
    RenderSettings& PolygonSmoothing(PolygonSmoothingConfig config);
    RenderSettings& Depth(DepthConfig config);
    RenderSettings& Blending(BlendingConfig config);
    RenderSettings& Culling(CullingConfig config);
    
    // Will only work if opengl context is set
    RenderSettings& Apply();


    void ShowRenderingWindow();
    void ShowInfoChild();
    void ShowSettingsChild();
    void ShowLogsChild();

private:
    ImGuiIO* m_imguiIO;
    RenderConfig m_config;
    WindowContext* m_context;

    void show_settings_tabs();
    void show_system_info(double updateRate = 0.08);
    void show_polygon_mode_controls();
    void show_depth_controls();
    void show_blending_controls();
    void show_culling_controls();
};