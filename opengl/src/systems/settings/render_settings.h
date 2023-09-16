#pragma once
#include "render_config.h"

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class OpenGLRenderSettings
{
public:
    OpenGLRenderSettings(const RenderConfig& config);
    OpenGLRenderSettings();


    OpenGLRenderSettings& PolygonMode(PolygonModeConfig config);
    OpenGLRenderSettings& PolygonSmoothing(PolygonSmoothingConfig config);
    OpenGLRenderSettings& Depth(DepthConfig config);
    OpenGLRenderSettings& Blending(BlendingConfig config);
    OpenGLRenderSettings& Culling(CullingConfig config);
    
    OpenGLRenderSettings& Apply();

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