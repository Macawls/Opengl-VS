
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "render_settings.h"
#include "../../utils/gl_call.h"

#include "../../utils/gui.h"

static RenderConfig previousConfig = RenderConfig();



void RenderSettings::ShowRenderingWindow()
{
    ImGui::SetNextWindowPos(ImVec2(m_imguiIO->DisplaySize.x - 500, 40));
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

void RenderSettings::ShowInfoChild()
{
    ImGui::BeginChild(WINDOW_HEADER, ImVec2(450, 200), false, ImGuiWindowFlags_NoCollapse );
    show_system_info();
    ImGui::EndChild();
}

void RenderSettings::ShowSettingsChild()
{
    ImGui::BeginChild(WINDOW_HEADER, ImVec2(400, 380), false);
    show_settings_tabs();
    ImGui::EndChild();
}

void RenderSettings::ShowLogsChild()
{
    ImGui::BeginChild(WINDOW_HEADER, ImVec2(500, 400), false);
    ShowLogsWindow();
    ImGui::EndChild();
}



RenderSettings::RenderSettings(const RenderConfig &config, WindowContext& context)
{
    m_context = &context;
    m_imguiIO = &ImGui::GetIO();
    this->Config = config;
    Apply();
}

RenderSettings::RenderSettings(WindowContext& context)
{
    m_context = &context;
    m_imguiIO = &ImGui::GetIO();
    this->Config = RenderConfig();
    Apply();
}

RenderSettings &RenderSettings::PolygonMode(PolygonModeConfig config)
{
    Config.PolygonMode = config;
    return *this;
}

RenderSettings &RenderSettings::PolygonSmoothing(PolygonSmoothingConfig config)
{
    Config.PolygonSmoothing = config;
    return *this;
}

RenderSettings &RenderSettings::Depth(DepthConfig config)
{
    Config.Depth = config;
    return *this;
}

RenderSettings &RenderSettings::Blending(BlendingConfig config)
{
    Config.Blending = config;
    return *this;
}

RenderSettings &RenderSettings::Culling(CullingConfig config)
{
    Config.Culling = config;
    return *this;
}

RenderSettings &RenderSettings::Apply()
{
    GLCall(glPolygonMode(Config.PolygonMode.Face, Config.PolygonMode.Mode));

    if (Config.Culling.Enabled)
    {
        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glCullFace(Config.Culling.Face));
        GLCall(glFrontFace(Config.Culling.WindingOrder));
    }
    else
    {
        GLCall(glDisable(GL_CULL_FACE));
    }

    if (Config.PolygonSmoothing.Enabled)
    {
        GLCall(glEnable(GL_POLYGON_SMOOTH));
        GLCall(glHint(GL_POLYGON_SMOOTH_HINT, Config.PolygonSmoothing.Hint));
    }
    else
    {
        GLCall(glDisable(GL_POLYGON_SMOOTH));
    }

    if (Config.Depth.Enabled)
    {
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glDepthFunc(Config.Depth.Function));
    }
    else
    {
        GLCall(glDisable(GL_DEPTH_TEST));
    }

    if (Config.Blending.Enabled)
    {
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(Config.Blending.SourceFactor, Config.Blending.DestinationFactor));
        GLCall(glBlendEquation(Config.Blending.Equation));
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
    if (ImGui::RadioButton("Fill##PolygonFill", Config.PolygonMode.Mode == GL_FILL))
    {
        Config.PolygonMode.Mode = GL_FILL;
        Apply();
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Line##PolygonLine", Config.PolygonMode.Mode == GL_LINE))
    {
        Config.PolygonMode.Mode = GL_LINE;
        Apply();
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Point##PolygonPoint", Config.PolygonMode.Mode == GL_POINT))
    {
        Config.PolygonMode.Mode = GL_POINT;
        Apply();
    }

    ImGui::Text("Face");
    if (ImGui::RadioButton("Front##PolygonFront", Config.PolygonMode.Face == GL_FRONT))
    {
        Config.PolygonMode.Face = GL_FRONT;
        Apply();
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Back##PolygonBack", Config.PolygonMode.Face == GL_BACK))
    {
        Config.PolygonMode.Face = GL_BACK;
        Apply();
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Front and Back##PolygonFrontAndBack", Config.PolygonMode.Face == GL_FRONT_AND_BACK))
    {
        Config.PolygonMode.Face = GL_FRONT_AND_BACK;
        Apply();
    }

    ImGui::Text("Smoothing");
    
    if (ImGui::Checkbox("Enabled##PolygonSmoothingEnabled", &Config.PolygonSmoothing.Enabled)) {
        Apply();
    }
    
  
    if (Config.PolygonSmoothing.Enabled)
    {
        if (ImGui::RadioButton("Fastest##PolygonSmoothingFastest", Config.PolygonSmoothing.Hint == GL_FASTEST))
        {
            Config.PolygonSmoothing.Hint = GL_FASTEST;
            Apply();
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Nicest##PolygonSmoothingNicest", Config.PolygonSmoothing.Hint == GL_NICEST))
        {
            Config.PolygonSmoothing.Hint = GL_NICEST;
            Apply();
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Dont Care##PolygonSmoothingDontCare", Config.PolygonSmoothing.Hint == GL_DONT_CARE))
        {
            Config.PolygonSmoothing.Hint = GL_DONT_CARE;
            Apply();
        }
    }
}

void RenderSettings::show_depth_controls()
{
    if (ImGui::Checkbox("Enabled##DepthEnabled", &Config.Depth.Enabled)) {
		Apply();
	}

    if (Config.Depth.Enabled)
    {
        if (ImGui::RadioButton("Never##DepthNever", Config.Depth.Function == GL_NEVER))
        {
            Config.Depth.Function = GL_NEVER;
            Apply();
        }

        if (ImGui::RadioButton("Less##DepthLess", Config.Depth.Function == GL_LESS))
        {
            Config.Depth.Function = GL_LESS;
            Apply();
        }

        if (ImGui::RadioButton("Equal##DepthEqual", Config.Depth.Function == GL_EQUAL))
        {
            Config.Depth.Function = GL_EQUAL;
            Apply();
        }

        if (ImGui::RadioButton("Less/Equal##DepthLessOrEqual", Config.Depth.Function == GL_LEQUAL))
        {
            Config.Depth.Function = GL_LEQUAL;
            Apply();
        }

        if (ImGui::RadioButton("Greater##DepthGreater", Config.Depth.Function == GL_GREATER))
        {
            Config.Depth.Function = GL_GREATER;
            Apply();
        }

        if (ImGui::RadioButton("Not Equal##DepthNotEqual", Config.Depth.Function == GL_NOTEQUAL))
        {
            Config.Depth.Function = GL_NOTEQUAL;
            Apply();
        }

        if (ImGui::RadioButton("Greater or Equal##DepthGreaterOrEqual", Config.Depth.Function == GL_GEQUAL))
        {
            Config.Depth.Function = GL_GEQUAL;
            Apply();
        }

        if (ImGui::RadioButton("Always##DepthAlways", Config.Depth.Function == GL_ALWAYS))
        {
            Config.Depth.Function = GL_ALWAYS;
            Apply();
        }
    }
}

void RenderSettings::show_blending_controls()
{
    if (ImGui::Checkbox("Enabled##BlendingEnabled", &Config.Blending.Enabled))
    {
       Apply();
    }

    if (Config.Blending.Enabled)
    {
        ImGui::Text("Source Factor");
        if (ImGui::RadioButton("GL_SRC_ALPHA", Config.Blending.SourceFactor == GL_SRC_ALPHA))
        {
            Config.Blending.SourceFactor = GL_SRC_ALPHA;
            Apply();
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("GL_ONE_MINUS_SRC_ALPHA##src_factor", Config.Blending.SourceFactor == GL_ONE_MINUS_SRC_ALPHA))
        {
            Config.Blending.SourceFactor = GL_ONE_MINUS_SRC_ALPHA;
            Apply();
        }

        ImGui::Text("Destination Factor");
        if (ImGui::RadioButton("GL_ONE", Config.Blending.DestinationFactor == GL_ONE))
        {
            Config.Blending.DestinationFactor = GL_ONE;
            Apply();
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("GL_ONE_MINUS_SRC_ALPHA##dest_factor", Config.Blending.DestinationFactor == GL_ONE_MINUS_SRC_ALPHA))
        {
            Config.Blending.DestinationFactor = GL_ONE_MINUS_SRC_ALPHA;
            Apply();
        }

        ImGui::Text("Blending Equation");
        if (ImGui::RadioButton("GL_FUNC_ADD", Config.Blending.Equation == GL_FUNC_ADD))
        {
            Config.Blending.Equation = GL_FUNC_ADD;
            Apply();
        }

        if (ImGui::RadioButton("GL_FUNC_SUBTRACT", Config.Blending.Equation == GL_FUNC_SUBTRACT))
        {
            Config.Blending.Equation = GL_FUNC_SUBTRACT;
            Apply();
        }

        if (ImGui::RadioButton("GL_FUNC_REVERSE_SUBTRACT", Config.Blending.Equation == GL_FUNC_REVERSE_SUBTRACT))
        {
            Config.Blending.Equation = GL_FUNC_REVERSE_SUBTRACT;
            Apply();
        }
    }
}

void RenderSettings::show_culling_controls()
{
    if (ImGui::Checkbox("Enabled##CullingEnabled", &Config.Culling.Enabled))
    {
        Apply();
    }

    if (Config.Culling.Enabled)
    {
        ImGui::Text("Culling Face");
        if (ImGui::RadioButton("GL_FRONT", Config.Culling.Face == GL_FRONT))
        {
            Config.Culling.Face = GL_FRONT;
            Apply();
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("GL_BACK", Config.Culling.Face == GL_BACK))
        {
            Config.Culling.Face = GL_BACK;
            Apply();
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("GL_FRONT_AND_BACK", Config.Culling.Face == GL_FRONT_AND_BACK))
        {
            Config.Culling.Face = GL_FRONT_AND_BACK;
            Apply();
        }

        ImGui::Text("Winding Order");
        if (ImGui::RadioButton("GL_CCW", Config.Culling.WindingOrder == GL_CCW))
        {
            Config.Culling.WindingOrder = GL_CCW;
            Apply();
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("GL_CW", Config.Culling.WindingOrder == GL_CW))
        {
            Config.Culling.WindingOrder = GL_CW;
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

            if (Config.PolygonMode != defaultConfig.PolygonMode)
            {
                if (ImGui::Button("Reset"))
                {
                    Config.PolygonMode = RenderConfig().PolygonMode;
                    Apply();
                }
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Depth"))
        {
            show_depth_controls();

            if (Config.Depth != defaultConfig.Depth)
            {
                if (ImGui::Button("Reset"))
                {
                    Config.Depth = RenderConfig().Depth;
                    Apply();
                }
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Blending"))
        {
            show_blending_controls();

            if (Config.Blending != defaultConfig.Blending)
            {
                if (ImGui::Button("Reset"))
                {
                    Config.Blending = RenderConfig().Blending;
                    Apply();
                }
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Culling"))
        {
            show_culling_controls();

            if (Config.Culling != defaultConfig.Culling)
            {
                if (ImGui::Button("Reset"))
                {
                    Config.Culling = RenderConfig().Culling;
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
    ImGui::Text("Cursor Pos        %.3f, %.3f", pos->X, pos->Y);
    ImGui::Text("Tri Count         %d", m_imguiIO->MetricsRenderIndices / 3);
}


