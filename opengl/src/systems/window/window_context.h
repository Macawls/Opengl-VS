#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../utils/logger.h"
#include "imgui_module.h"

#include <functional>

struct WindowStatus
{
    bool isFullscreen = false;
    bool isMinimized = false;
    bool isVSync = false;
};

struct WindowParameters
{
    int width;
    int height;
    const char *title;
    float imguiFontSize;
    GLFWimage iconImage;
};

struct FramebufferSize
{
    int width;
    int height;
};


// (1, 1) is top right, (-1, -1) is bottom left
struct NormalizedMousePosition
{
	float x;
	float y;
};

// A Wrapper for GLFWwindow, also manages ImGui and OpenGL context
class WindowContext
{
public:
    WindowContext(WindowParameters params);
    ~WindowContext();
    ImGUIModule ImGUI = ImGUIModule();
    
    // Read only
    WindowStatus WindowStatus;
    
    // Sets the update function, called every frame
    void SetDeltaUpdate(std::function<void(float deltaTime)> func) { this->m_update = func; }
    
    // Closes the window
    void Close();
    // Begins the main loop, update function is called every frame
    void BeginLoop();

    // Toggle's VSync, static because there's only one window really
    static void ToggleVSync(bool enable) { glfwSwapInterval(enable ? 1 : 0); }

    // Returns the GLFWwindow pointer
    GLFWwindow *GetGLFWWindow() const { return m_window; }
    
    // Returns the video mode of the primary monitor
    static GLFWvidmode GetVideoMode() { return *glfwGetVideoMode(glfwGetPrimaryMonitor()); }
    
    // Returns the framebuffer size
    FramebufferSize* GetFrameBufferSize() { return &m_FramebufferSize; }

    // Returns the normalized mouse position
    NormalizedMousePosition* GetNormalizedMousePosition() { return &m_normalizedMousePosition; }
    
    // Returns the framebuffer aspect ratio
    float GetFrameBufferAspectRatio() { return (float)m_FramebufferSize.width / (float)m_FramebufferSize.height; }
private:
    GLFWwindow *m_window;
    

    std::function<void(float deltaTime)> m_update;
    FramebufferSize m_FramebufferSize;
    NormalizedMousePosition m_normalizedMousePosition;
    
    bool init(WindowParameters params);
    void set_standard_callbacks();
    void set_standard_window_hints();
};
