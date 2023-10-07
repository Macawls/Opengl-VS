
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "render_settings.h"
#include "../../utils/gl_call.h"
#include "../resources/fonts/forkawesome-icons.h"
#include "../../utils/gui.h"

static RenderConfig previousConfig = RenderConfig();

static const char* SETTINGS_HEADER = ICON_FK_COGS " Settings";
static const char* INFO_HEADER = ICON_FK_TELEVISION " Information";
static const char* LOGS_HEADER = ICON_FK_BARS " Logs";
static const char* CONFIG_HEADER = ICON_FK_WRENCH " Configuration";
static const char* WINDOW_HEADER = "System";

void RenderSettings::ShowRenderingWindow()
{
    ImGui::SetNextWindowPos(ImVec2(m_imguiIO->DisplaySize.x - 490, 20));
    ImGui::SetNextWindowSize(ImVec2(470, 0));
    ImGui::Begin(WINDOW_HEADER);

    if (ImGui::BeginTabBar("SystemTabs")) {

        if (ImGui::BeginTabItem(CONFIG_HEADER)) {
			
            if (ImGui::CollapsingHeader(INFO_HEADER, ImGuiTreeNodeFlags_DefaultOpen)) {
                show_system_info();
            }

            if (ImGui::CollapsingHeader(SETTINGS_HEADER, ImGuiTreeNodeFlags_DefaultOpen)) {
                show_settings_tabs();
            }
        
			ImGui::EndTabItem();
		}

        if (ImGui::BeginTabItem(LOGS_HEADER)) {
			ShowLogsWindow();
			ImGui::EndTabItem();
		}

        ImGui::EndTabBar();
    }

    ImGui::End();
}


RenderSettings::RenderSettings(const RenderConfig &config, WindowContext& context)
{
    m_context = &context;
    m_imguiIO = &ImGui::GetIO();
    this->m_config = config;
    Apply();
}

RenderSettings::RenderSettings(WindowContext& context)
{
    m_context = &context;
    m_imguiIO = &ImGui::GetIO();
    this->m_config = RenderConfig();
    Apply();
}

RenderSettings &RenderSettings::PolygonMode(PolygonModeConfig config)
{
    m_config.polygonMode = config;
    return *this;
}

RenderSettings &RenderSettings::PolygonSmoothing(PolygonSmoothingConfig config)
{
    m_config.polygonSmoothing = config;
    return *this;
}

RenderSettings &RenderSettings::Depth(DepthConfig config)
{
    m_config.depth = config;
    return *this;
}

RenderSettings &RenderSettings::Blending(BlendingConfig config)
{
    m_config.blending = config;
    return *this;
}

RenderSettings &RenderSettings::Culling(CullingConfig config)
{
    m_config.culling = config;
    return *this;
}

RenderSettings &RenderSettings::Apply()
{
    GLCall(glPolygonMode(m_config.polygonMode.face, m_config.polygonMode.mode));

    if (m_config.culling.enabled)
    {
        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glCullFace(m_config.culling.face));
        GLCall(glFrontFace(m_config.culling.windingOrder));
    }
    else
    {
        GLCall(glDisable(GL_CULL_FACE));
    }

    if (m_config.polygonSmoothing.enabled)
    {
        GLCall(glEnable(GL_POLYGON_SMOOTH));
        GLCall(glHint(GL_POLYGON_SMOOTH_HINT, m_config.polygonSmoothing.hint));
    }
    else
    {
        GLCall(glDisable(GL_POLYGON_SMOOTH));
    }

    if (m_config.depth.enabled)
    {
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glDepthFunc(m_config.depth.function));
    }
    else
    {
        GLCall(glDisable(GL_DEPTH_TEST));
    }

    if (m_config.blending.enabled)
    {
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(m_config.blending.sourceFactor, m_config.blending.destinationFactor));
        GLCall(glBlendEquation(m_config.blending.equation));
    }
    else
    {
        GLCall(glDisable(GL_BLEND));
    }

    return *this;
}

void RenderSettings::show_polygon_mode_controls()
{
    ImGui::Text("Mode");
    if (ImGui::RadioButton("Fill##PolygonFill", m_config.polygonMode.mode == GL_FILL))
    {
        m_config.polygonMode.mode = GL_FILL;
        Apply();
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Line##PolygonLine", m_config.polygonMode.mode == GL_LINE))
    {
        m_config.polygonMode.mode = GL_LINE;
        Apply();
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Point##PolygonPoint", m_config.polygonMode.mode == GL_POINT))
    {
        m_config.polygonMode.mode = GL_POINT;
        Apply();
    }

    ImGui::Text("Face");
    if (ImGui::RadioButton("Front##PolygonFront", m_config.polygonMode.face == GL_FRONT))
    {
        m_config.polygonMode.face = GL_FRONT;
        Apply();
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Back##PolygonBack", m_config.polygonMode.face == GL_BACK))
    {
        m_config.polygonMode.face = GL_BACK;
        Apply();
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Front and Back##PolygonFrontAndBack", m_config.polygonMode.face == GL_FRONT_AND_BACK))
    {
        m_config.polygonMode.face = GL_FRONT_AND_BACK;
        Apply();
    }

    ImGui::Text("Smoothing");
    
    if (ImGui::Checkbox("Enabled##PolygonSmoothingEnabled", &m_config.polygonSmoothing.enabled)) {
        Apply();
    }
    
  
    if (m_config.polygonSmoothing.enabled)
    {
        if (ImGui::RadioButton("Fastest##PolygonSmoothingFastest", m_config.polygonSmoothing.hint == GL_FASTEST))
        {
            m_config.polygonSmoothing.hint = GL_FASTEST;
            Apply();
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Nicest##PolygonSmoothingNicest", m_config.polygonSmoothing.hint == GL_NICEST))
        {
            m_config.polygonSmoothing.hint = GL_NICEST;
            Apply();
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Dont Care##PolygonSmoothingDontCare", m_config.polygonSmoothing.hint == GL_DONT_CARE))
        {
            m_config.polygonSmoothing.hint = GL_DONT_CARE;
            Apply();
        }
    }
}

void RenderSettings::show_depth_controls()
{
    if (ImGui::Checkbox("Enabled##DepthEnabled", &m_config.depth.enabled)) {
		Apply();
	}

    if (m_config.depth.enabled)
    {
        if (ImGui::RadioButton("Never##DepthNever", m_config.depth.function == GL_NEVER))
        {
            m_config.depth.function = GL_NEVER;
            Apply();
        }

        if (ImGui::RadioButton("Less##DepthLess", m_config.depth.function == GL_LESS))
        {
            m_config.depth.function = GL_LESS;
            Apply();
        }

        if (ImGui::RadioButton("Equal##DepthEqual", m_config.depth.function == GL_EQUAL))
        {
            m_config.depth.function = GL_EQUAL;
            Apply();
        }

        if (ImGui::RadioButton("Less/Equal##DepthLessOrEqual", m_config.depth.function == GL_LEQUAL))
        {
            m_config.depth.function = GL_LEQUAL;
            Apply();
        }

        if (ImGui::RadioButton("Greater##DepthGreater", m_config.depth.function == GL_GREATER))
        {
            m_config.depth.function = GL_GREATER;
            Apply();
        }

        if (ImGui::RadioButton("Not Equal##DepthNotEqual", m_config.depth.function == GL_NOTEQUAL))
        {
            m_config.depth.function = GL_NOTEQUAL;
            Apply();
        }

        if (ImGui::RadioButton("Greater or Equal##DepthGreaterOrEqual", m_config.depth.function == GL_GEQUAL))
        {
            m_config.depth.function = GL_GEQUAL;
            Apply();
        }

        if (ImGui::RadioButton("Always##DepthAlways", m_config.depth.function == GL_ALWAYS))
        {
            m_config.depth.function = GL_ALWAYS;
            Apply();
        }
    }
}

void RenderSettings::show_blending_controls()
{
    if (ImGui::Checkbox("Enabled##BlendingEnabled", &m_config.blending.enabled))
    {
       Apply();
    }

    if (m_config.blending.enabled)
    {
        ImGui::Text("Source Factor");
        if (ImGui::RadioButton("GL_SRC_ALPHA", m_config.blending.sourceFactor == GL_SRC_ALPHA))
        {
            m_config.blending.sourceFactor = GL_SRC_ALPHA;
            Apply();
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("GL_ONE_MINUS_SRC_ALPHA##src_factor", m_config.blending.sourceFactor == GL_ONE_MINUS_SRC_ALPHA))
        {
            m_config.blending.sourceFactor = GL_ONE_MINUS_SRC_ALPHA;
            Apply();
        }

        ImGui::Text("Destination Factor");
        if (ImGui::RadioButton("GL_ONE", m_config.blending.destinationFactor == GL_ONE))
        {
            m_config.blending.destinationFactor = GL_ONE;
            Apply();
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("GL_ONE_MINUS_SRC_ALPHA##dest_factor", m_config.blending.destinationFactor == GL_ONE_MINUS_SRC_ALPHA))
        {
            m_config.blending.destinationFactor = GL_ONE_MINUS_SRC_ALPHA;
            Apply();
        }

        ImGui::Text("Blending Equation");
        if (ImGui::RadioButton("GL_FUNC_ADD", m_config.blending.equation == GL_FUNC_ADD))
        {
            m_config.blending.equation = GL_FUNC_ADD;
            Apply();
        }

        if (ImGui::RadioButton("GL_FUNC_SUBTRACT", m_config.blending.equation == GL_FUNC_SUBTRACT))
        {
            m_config.blending.equation = GL_FUNC_SUBTRACT;
            Apply();
        }

        if (ImGui::RadioButton("GL_FUNC_REVERSE_SUBTRACT", m_config.blending.equation == GL_FUNC_REVERSE_SUBTRACT))
        {
            m_config.blending.equation = GL_FUNC_REVERSE_SUBTRACT;
            Apply();
        }
    }
}

void RenderSettings::show_culling_controls()
{
    if (ImGui::Checkbox("Enabled##CullingEnabled", &m_config.culling.enabled))
    {
        Apply();
    }

    if (m_config.culling.enabled)
    {
        ImGui::Text("Culling Face");
        if (ImGui::RadioButton("GL_FRONT", m_config.culling.face == GL_FRONT))
        {
            m_config.culling.face = GL_FRONT;
            Apply();
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("GL_BACK", m_config.culling.face == GL_BACK))
        {
            m_config.culling.face = GL_BACK;
            Apply();
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("GL_FRONT_AND_BACK", m_config.culling.face == GL_FRONT_AND_BACK))
        {
            m_config.culling.face = GL_FRONT_AND_BACK;
            Apply();
        }

        ImGui::Text("Winding Order");
        if (ImGui::RadioButton("GL_CCW", m_config.culling.windingOrder == GL_CCW))
        {
            m_config.culling.windingOrder = GL_CCW;
            Apply();
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("GL_CW", m_config.culling.windingOrder == GL_CW))
        {
            m_config.culling.windingOrder = GL_CW;
            Apply();
        }
    }
}

void RenderSettings::show_settings_tabs()
{
    // default config used for comparison
    static RenderConfig defaultConfig = RenderConfig();
    // Note on imgui, syntax means {display}##{internal id}

    if (ImGui::BeginTabBar("Render Settings"))
    {
        if (ImGui::BeginTabItem("Polygon"))
        {
            show_polygon_mode_controls();

            if (m_config.polygonMode != defaultConfig.polygonMode)
            {
                if (ImGui::Button("Reset"))
                {
                    m_config.polygonMode = RenderConfig().polygonMode;
                    Apply();
                }
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Depth"))
        {
            show_depth_controls();

            if (m_config.depth != defaultConfig.depth)
            {
                if (ImGui::Button("Reset"))
                {
                    m_config.depth = RenderConfig().depth;
                    Apply();
                }
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Blending"))
        {
            show_blending_controls();

            if (m_config.blending != defaultConfig.blending)
            {
                if (ImGui::Button("Reset"))
                {
                    m_config.blending = RenderConfig().blending;
                    Apply();
                }
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Culling"))
        {
            show_culling_controls();

            if (m_config.culling != defaultConfig.culling)
            {
                if (ImGui::Button("Reset"))
                {
                    m_config.culling = RenderConfig().culling;
                    Apply();
                }
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::Dummy(ImVec2(0.0f, 15.0f));
}

void RenderSettings::show_system_info(double updateRate)
{
    static double lastUpdateTime = 0.0;
    double currentTime = glfwGetTime();
    static float lastFrameTime = 0.0f;
    static float lastFPS = 0.0f;
    static bool isVSync = false;

    static double mouseX;
    static double mouseY;

    if (currentTime - lastUpdateTime >= updateRate)
    {
        lastFPS = m_imguiIO->Framerate;
        lastFrameTime = 1000.0f / m_imguiIO->Framerate;
        lastUpdateTime = currentTime;
    }

    
    ImGui::Text("Renderer          %s", glGetString(GL_RENDERER));
    ImGui::Text("FPS               %.2f", lastFPS);
    ImGui::SameLine();
    ImGui::Dummy(ImVec2(80.0f, 0.0f));
    ImGui::SameLine();
    if (ImGui::Checkbox("VSync", &isVSync)) { WindowContext::ToggleVSync(isVSync); }
    
    
    ImGui::Text("Frametime         %.3fms", lastFrameTime);
    ImGui::Text("Window Size       %.0fpx X %.0fpx ", m_imguiIO->DisplaySize.x, m_imguiIO->DisplaySize.y);
    NormalizedMousePosition* pos = m_context->GetNormalizedMousePosition();
    ImGui::Text("Cursor Pos        %.3f, %.3f", pos->x, pos->y);
    ImGui::Text("Tri Count         %d", m_imguiIO->MetricsRenderIndices / 3);
}


