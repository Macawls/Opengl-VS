#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm> //std::clamp

#include "../window/window_context.h"
#include <GLFW/glfw3.h>


#include "transform.h"

// Notes: aspect ratio of window size = stretched, aspect ratio of framebuffer size = no stretching
// TODO: should rather use events for updating aspect ratio instead of every frame, fine for now
// Handling Inputs needs to be called before OnUpdate

struct ClipPlane
{
	float Near = 0.1f;
	float Far = 200.0f;
};

struct CameraSettings
{
    float Yaw = -90.0f;
    float Pitch =  0.0f;
    float Fov = 60.0f;
    float MovementSpeed = 2.5f;
    float LookSpeed = 60.0f;
    ClipPlane ClippingPlane;
    bool Unlocked;
};

/*
enum CameraMode
{
    PAN, ROTATE
};
*/

/*
Front View
0.0f, 0.0f, 3.0f Pos
0.0f, 0.0f, 0.0f Rot

Right View
3.0f, 0.0f, 0.0f Pos
0.0f, -90f, 0.0 Rot

Left View
- 3.0f, 0.0f, 0.0f Pos
0.0f, 90f, 0.0f Rot

Back View
0.0f, 0.0f, 0.0f Pos
0.0f, 180.Of, 0.0f Rot
*/


// N.B Set WindowContext before using
class PerspectiveCamera
{
public:
    inline static glm::vec3 STARTING_POSITION = glm::vec3(0.0f, 0.0f, 3.0f);
    inline static const char* GUI_HEADER = ICON_FK_CAMERA " Camera";

    PerspectiveCamera(WindowContext& windowContext, const glm::vec3& position = STARTING_POSITION);
    CameraSettings Settings;
    TransformComponent Transform = TransformComponent().SetPosition(STARTING_POSITION);

    glm::mat4 GetViewMatrix() const;
    
    glm::mat4 GetProjectionMatrix() const { return m_projectionMatrix; }

    glm::mat4 CalculateMvp(const TransformComponent& transform) const;

    void OnUpdate(float deltaTime);

    void HandleKeyInput(float deltaTime);
    void HandleMouseInput(float deltaTime);
    void GuiShowControls();

private:
    WindowContext* m_context;
    GLFWwindow* m_window;
   

    float m_aspectRatio;
    double m_lastMouseX, m_lastMouseY = 0.0;

    
    const glm::vec3 m_worldRight = glm::vec3(1.0f, 0.0f, 0.0f);
    const glm::vec3 m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::vec3 m_worldFront = glm::vec3(0.0f, 0.0f, -1.0f);
    

    glm::vec3 m_right = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);

    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewMatrix;

    
    void update_vectors();
    void update_projection_matrix();
    void update_view_matrix();
    void on_unlocked(float deltaTime);
};