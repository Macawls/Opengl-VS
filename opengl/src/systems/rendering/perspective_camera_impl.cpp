#include "perspective_camera.h"

#include <glm/gtc/constants.hpp>

PerspectiveCamera::PerspectiveCamera(WindowContext& windowContext, glm::vec3 Position)
{
    if (Position != CAMERA_STARTING_POSITION) {
        Transform.Position = Position;
    }
    
    m_context = &windowContext;
    m_window = m_context->GetGLFWWindow();

    update_projection_matrix();
    update_view_matrix();
}

glm::mat4 PerspectiveCamera::GetViewMatrix() const
{
    return m_viewMatrix;
}

void PerspectiveCamera::update_projection_matrix()
{
    m_aspectRatio = m_context->GetFrameBufferAspectRatio();
    
    m_projectionMatrix = glm::perspective(
        glm::radians(Settings.Fov), 
        m_aspectRatio,
        ClippingPlane.Near, 
        ClippingPlane.Far);
}

void PerspectiveCamera::update_view_matrix() {
    
    m_viewMatrix = glm::mat4(1.0f);
    
    // First, apply the rotations around the origin
    m_viewMatrix = glm::rotate(m_viewMatrix, glm::radians(Transform.Rotation.x), m_x_axis);
    m_viewMatrix = glm::rotate(m_viewMatrix, glm::radians(Transform.Rotation.y), m_y_axis);
    m_viewMatrix = glm::rotate(m_viewMatrix, glm::radians(Transform.Rotation.z), m_z_axis);

    // Then, apply the translation to move the camera to its position
    m_viewMatrix = glm::translate(m_viewMatrix, -Transform.Position);
}

void PerspectiveCamera::HandleKeyInput(float deltaTime)
{
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
    {
        Transform.Position += Settings.Speed * deltaTime * m_front;
    }

    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
    {
        Transform.Position -= Settings.Speed * deltaTime * m_front;
    }

    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
    {
        Transform.Position -= glm::normalize(glm::cross(m_front, m_up)) * Settings.Speed * deltaTime;
    }

    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
    {
        Transform.Position += glm::normalize(glm::cross(m_front, m_up)) * Settings.Speed * deltaTime;
    }
}

void PerspectiveCamera::HandleMouseInput(float deltaTime)
{
    // [LEFT, BOTTOM] = [-1, -1], [RIGHT, TOP] = [1, 1]
    NormalizedMousePosition* pos = m_context->GetNormalizedMousePosition();

    // Calculate the mouse movement
    float deltaX = pos->x - m_prevMouseX;
    float deltaY = pos->y - m_prevMouseY;

    if (deltaX == 0 && deltaY == 0) return;

    if (m_mode == CameraMode::PAN)
    {
        //Transform.Position += glm::vec3(0.0f, deltaY * Settings.Speed, 0.0f);
        //Transform.Position += glm::vec3(-deltaX * Settings.Speed, 0.0f, 0.0f);
    }

    if (m_mode == CameraMode::ROTATE) 
    {
        
    }

    m_prevMouseX = pos->x;
    m_prevMouseY = pos->y;
}


