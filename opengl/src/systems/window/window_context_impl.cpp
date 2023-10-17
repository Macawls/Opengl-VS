#include "window_context.h"
#include "../../utils/logger.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#define CLASS_NAME(className) #className

WindowContext::WindowContext(WindowParameters params)
{
    if (!init(params))
    {
        Logger::LogError("%s Init failed", CLASS_NAME(WindowContext));
        glfwTerminate();
        return;
    }

    glfwSetWindowIcon(m_window, 1, &params.IconImage);

    Logger::LogDebug("%s Init Sucess", CLASS_NAME(WindowContext));
    Logger::LogDebug("%s Init Success, version: %s", CLASS_NAME(ImGUIModule),  IMGUI_VERSION);
    
    Logger::LogDebug("OpenGL Version: %s", glGetString(GL_VERSION));
    Logger::LogDebug("OpenGL Vendor: %s", glGetString(GL_VENDOR));
    Logger::LogDebug("OpenGL Renderer: %s", glGetString(GL_RENDERER));
}

WindowContext::~WindowContext()
{
    // imgui shutdown is already called in it's destructor
    //glfwDestroyWindow(m_window);
    glfwTerminate();
}

void WindowContext::BeginLoop()
{
    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();

        // Prevents crash when framebuffer is 0
        if (WindowStatus.IsMinimized)
        {
            glfwWaitEvents();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            continue;
        }

        ImGui.BeginFrame();
        m_update(ImGui::GetIO().DeltaTime);
        ImGui.EndFrame();

        glfwSwapBuffers(m_window);
    }
}

bool WindowContext::init(WindowParameters params)
{
    if (!glfwInit())
    {
        Logger::LogError("Failed to initialize GLFW");
        return false;
    }

    // Before window creation
    set_standard_window_hints();

    m_window = glfwCreateWindow(params.Width, params.Height, params.Title, nullptr, nullptr);

    if (!m_window)
    {
        Logger::LogError("Failed to create GLFW window");
        return false;
    }

    // Get buffer size information
    glfwGetFramebufferSize(m_window, &m_framebufferSize.Width, &m_framebufferSize.Height);
    // Set viewport
    glViewport(0, 0, m_framebufferSize.Width, m_framebufferSize.Height);
    // Set opengl context, before glew and imgui init
    glfwMakeContextCurrent(m_window);
    // Set user pointer to this class, so we can access it in callbacks
    glfwSetWindowUserPointer(m_window, this);

    
    // Init GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        Logger::LogError("GLEW Init Failed, Error: %s", glewGetErrorString(err));
        return false;
    }

    // Before imgui init
    set_standard_callbacks();

    // Init ImGui with window context
    bool success = ImGui.Init("#version 330", m_window, params.ImguiFontSize);
    if (!success)
    {
        Logger::LogError("%s Init Failed", CLASS_NAME(ImGUIModule));
        return false;
    }

    return true;
}

// N.B always call set_standard_callbacks() 
// BEFORE IMGUI INIT
void WindowContext::set_standard_callbacks()
{
    // Make sure viewport updates when window is resized
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *window, int width, int height)
    {
        WindowContext *win = (WindowContext *)glfwGetWindowUserPointer(window);
        
        win->m_framebufferSize.Width = width;
        win->m_framebufferSize.Height = height;

        glViewport(0, 0, width, height); 
    });

    // Update normalized mouse position
    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos)
    {
		WindowContext *win = (WindowContext *)glfwGetWindowUserPointer(window);
		
        // * 2.0 - 1.0f to get range [-1, 1]
        win->m_normalizedMousePosition.X = (static_cast<float>(xpos) / (float)win->m_framebufferSize.Width) * 2.0 - 1.0f;
		win->m_normalizedMousePosition.Y = (1 - (static_cast<float>(ypos) / (float)win->m_framebufferSize.Height)) * 2.0f - 1.0f;
	}); 

    // Set sleep status when window is minimized
    glfwSetWindowIconifyCallback(m_window, [](GLFWwindow *window, int iconified)
    {
        WindowContext *win = (WindowContext *)glfwGetWindowUserPointer(window);
        win->WindowStatus.IsMinimized = iconified;
    });

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow *window)
    {
        WindowContext *win = (WindowContext *)glfwGetWindowUserPointer(window);
        win->Close(); 
    });
}

void WindowContext::set_standard_window_hints()
{
    // Request high OpenGL version (4.6)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    // Use core profile to avoid deprecated features
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Makewindow resizable
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // Enable forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    // Enable MSAA (Multisample Anti-Aliasing)
    // glfwWindowHint(GLFW_SAMPLES, 4);

    // Set the GLFW context creation to be more robust
    glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_RELEASE_BEHAVIOR_FLUSH);

    // Disable double buffering
    // glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
}

void WindowContext::Close()
{
    glfwSetWindowShouldClose(m_window, GL_TRUE);
}