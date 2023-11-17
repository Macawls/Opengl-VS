#include "perspective_camera.h"
#include <glm/gtc/constants.hpp>

// Accessed 10 November
// https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/camera.h

PerspectiveCamera::PerspectiveCamera(WindowContext& windowContext, const glm::vec3& position)
{
    if (position != STARTING_POSITION) {
        Transform.Position = position;
    }
    
    m_context = &windowContext;
    m_window = m_context->GetGlfwWindow();

    update_projection_matrix();
    update_view_matrix();

    /*
    glfwSetCursorPosCallback(m_window, [](GLFWwindow *window, double xPosIn, double yPosIn)
    {
        
    });
    */
}

void PerspectiveCamera::update_projection_matrix()
{
    m_aspectRatio = m_context->GetFrameBufferAspectRatio();
    
    m_projectionMatrix = glm::perspective(
        glm::radians(Settings.Fov), 
        m_aspectRatio,
        Settings.ClippingPlane.Near, 
        Settings.ClippingPlane.Far);
}

void PerspectiveCamera::update_view_matrix()
{
    m_viewMatrix = lookAt(Transform.Position, Transform.Position + Front, Up);
}

void PerspectiveCamera::update_vectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Settings.Yaw)) * cos(glm::radians(Settings.Pitch));
    front.y = sin(glm::radians(Settings.Pitch));
    front.z = sin(glm::radians(Settings.Yaw)) * cos(glm::radians(Settings.Pitch));

    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Right = glm::normalize(glm::cross(Front, m_worldUp));  
    Up    = glm::normalize(glm::cross(Right, Front));
}

glm::mat4 PerspectiveCamera::GetViewMatrix() const
{
    return m_viewMatrix;
}
glm::mat4 PerspectiveCamera::CalculateMvp(const TransformComponent& transform) const {
    return GetProjectionMatrix() * GetViewMatrix() * transform.GetModelMatrix();
}

void PerspectiveCamera::OnUpdate(float deltaTime)
{
    update_projection_matrix();
    update_vectors();
    update_view_matrix();
}

void PerspectiveCamera::HandleKeyInput(float deltaTime)
{
    // Accessed 15 November 2023
    // https://stackoverflow.com/questions/52492426/glfw-switching-boolean-toggle
    static bool unlockKeyPressed = false;
    
    if (glfwGetKey(m_window, GLFW_KEY_TAB) == GLFW_PRESS)
    {
        if (!unlockKeyPressed)
        {
            Settings.Unlocked = !Settings.Unlocked;
            unlockKeyPressed = true;
        }
    }
    else if (glfwGetKey(m_window, GLFW_KEY_TAB) == GLFW_RELEASE)
    {
        unlockKeyPressed = false;
    }

    if (!Settings.Unlocked) return;
    on_unlocked(deltaTime);
}

void PerspectiveCamera::on_unlocked(float deltaTime)
{
    // forward
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        Transform.Translate(Settings.MovementSpeed * deltaTime * Front);
    }
    // back
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        Transform.Translate(Settings.MovementSpeed * deltaTime * -Front);
    }
    // left
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        const auto dir = normalize(cross(Front, Up));
        Transform.Translate(-dir * Settings.MovementSpeed * deltaTime);
    }
    // right
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        const auto dir = normalize(cross(Front, Up));
        Transform.Translate(dir * Settings.MovementSpeed * deltaTime);
    }
}

void PerspectiveCamera::HandleMouseInput(float deltaTime)
{
    // [LEFT, BOTTOM] = [-1, -1], [RIGHT, TOP] = [1, 1]
    const NormalizedMousePosition* pos = m_context->GetNormalizedMousePosition();

    static glm::vec2 prevMousePosition;

    // calc mouse delta
    const auto mouseDelta = glm::vec2(pos->X - prevMousePosition.x, pos->Y - prevMousePosition.y);

    // update prev
    prevMousePosition = glm::vec2(pos->X, pos->Y);

    if (!Settings.Unlocked) return;
    
    Settings.Yaw += mouseDelta.x * Settings.LookSpeed;
    Settings.Pitch += mouseDelta.y * Settings.LookSpeed;

    // Clamp pitch to the range [-89.0f, 89.0f]
    Settings.Pitch = glm::clamp(Settings.Pitch, -89.0f, 89.0f);
    update_vectors();
}


void PerspectiveCamera::GuiShowControls()
{
    GuiShowTitle(0.1f, GUI_HEADER);

    ImGui::Columns(2, "Clipping Plane Column", true);
    ImGui::SetColumnOffset(0, -130.0f);
    ImGui::Text("Settings");
    ImGui::Text("TAB to toggle");
    ImGui::NextColumn();
    ImGui::SliderFloat("FOV", &Settings.Fov, 5.0f, 140.0f);
    ImGui::Checkbox("Unlocked", &Settings.Unlocked);
    ImGui::SliderFloat("Move Speed", &Settings.MovementSpeed, 2.0f, 10.0f);
    ImGui::SliderFloat("Look Speed", &Settings.LookSpeed, 1.0f, 90.0f);
    ImGui::Columns(1);
    

    ImGui::Columns(2, "Clipping Plane Column", true);
    ImGui::Text("Clipping");
    ImGui::NextColumn();
    const float farMin = Settings.ClippingPlane.Near + 0.1f;
    ImGui::DragFloat("##ClippingPlaneNear", &Settings.ClippingPlane.Near, 0.03f, 0.01f, 30.0f, "Near %.2f",
        ImGuiSliderFlags_Logarithmic);
    ImGui::DragFloat("##ClippingPlaneFar", &Settings.ClippingPlane.Far, 0.1f, farMin, 800.0f, "Far %.2f");
    ImGui::Columns(1);

    ImGui::Columns(2, "Clipping Plane Column", true);
    ImGui::Text("Orientation");
    ImGui::NextColumn();
    ImGui::DragFloat("##Yaw", &Settings.Pitch, 0.1f, -89.0f, 89.0f, "Pitch %.2f");
    ImGui::DragFloat("##Pitch", &Settings.Yaw, 0.1f, -500, 500, "Yaw %.2f");
    ImGui::Columns(1);
    ImGui::Separator();

    Transform.GuiShowControlsPosition(STARTING_POSITION);
    
    if (ImGui::Button("Reset"))
    {
        Settings = CameraSettings();
    }
}


