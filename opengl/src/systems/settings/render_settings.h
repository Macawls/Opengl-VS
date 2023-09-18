#pragma once
#include "configs.h"

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
    RenderSettings(const RenderConfig& config, bool applyDefaults);
    RenderSettings(bool applyDefaults);


    RenderSettings& PolygonMode(PolygonModeConfig config);
    RenderSettings& PolygonSmoothing(PolygonSmoothingConfig config);
    RenderSettings& Depth(DepthConfig config);
    RenderSettings& Blending(BlendingConfig config);
    RenderSettings& Culling(CullingConfig config);
    
    RenderSettings& Apply();

    void ShowImGuiTabBar();
    void ShowInfo(double updateRate = 0.08);
private:
    ImGuiIO* m_io;
    RenderConfig m_config;
    void show_polygon_mode_controls();
    void show_depth_controls();
    void show_blending_controls();
    void show_culling_controls();
};