#pragma once


#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include "imgui_module.h"

#include <functional>

struct WindowStatus
{
    bool IsFullscreen = false;
    bool IsMinimized = false;
    bool IsVSync = false;
};

struct WindowParameters
{
    int Width;
    int Height;
    const char *Title;
    float ImguiFontSize;
    GLFWimage IconImage;
};

struct FramebufferSize
{
    int Width;
    int Height;
};


// (1, 1) is top right, (-1, -1) is bottom left
struct NormalizedMousePosition
{
	float X;
	float Y;
};

// A Wrapper for GLFWwindow, also manages ImGui and OpenGL context
class WindowContext
{
    
public:
    WindowContext(WindowParameters params);
    ~WindowContext();
    ImGUIModule ImGui = ImGUIModule();
    
    // Read only
    WindowStatus WindowStatus;
    
    // Sets the update function, called every frame
    void SetDeltaUpdate(const std::function<void(float deltaTime)>& func)
    {
        this->m_update = func;
    }
    
    // Closes the window
    void Close();
    // Begins the main loop, update function is called every frame
    void BeginLoop();

    // Toggle's VSync, static because there's only one window really
    static void ToggleVSync(const bool enable) { glfwSwapInterval(enable ? 1 : 0); }

    // Returns the GLFWwindow pointer
    GLFWwindow *GetGlfwWindow() const { return m_window; }
    
    // Returns the video mode of the primary monitor
    static GLFWvidmode GetVideoMode() { return *glfwGetVideoMode(glfwGetPrimaryMonitor()); }
    
    // Returns the framebuffer size
    FramebufferSize* GetFrameBufferSize() { return &m_framebufferSize; }

    // Returns the normalized mouse position
    NormalizedMousePosition* GetNormalizedMousePosition() { return &m_normalizedMousePosition; }
    
    // Returns the framebuffer aspect ratio
    float GetFrameBufferAspectRatio() const { return (float)m_framebufferSize.Width / (float)m_framebufferSize.Height; }

    // Changes title of the window
    void SetWindowTitle(const char* title) const { glfwSetWindowTitle(m_window, title); }

private:
    GLFWwindow *m_window;
    

    std::function<void(float deltaTime)> m_update;
    FramebufferSize m_framebufferSize;
    NormalizedMousePosition m_normalizedMousePosition;
    
    bool init(WindowParameters params);
    void set_standard_callbacks();
    void set_standard_window_hints();
};
