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
    float Fov = 60.0f;
    float Speed = 3.0f;
    float Sensitivity = 30.0f;
    ClipPlane ClippingPlane = ClipPlane();
};

enum CameraMode
{
    PAN, ROTATE
};

#define CAMERA_STARTING_POSITION glm::vec3(0.0f, 0.0f, 3.0f)

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
    PerspectiveCamera(WindowContext& windowContext, const glm::vec3& position = CAMERA_STARTING_POSITION);

    CameraSettings Settings;
    TransformComponent Transform = TransformComponent().SetPosition(CAMERA_STARTING_POSITION);

    glm::mat4 GetViewMatrix() const;

    glm::mat4 GetProjectionMatrix() const { return m_projectionMatrix; }

    glm::mat4 CalculateMvp(const TransformComponent& transform) const;

    void OnUpdate(float deltaTime)
    {
        update_projection_matrix();
        update_view_matrix();
    }

    void Reset() {
        Transform.Reset();
        Settings = CameraSettings();
    }

    void HandleKeyInput(float deltaTime);
    void HandleMouseInput(float deltaTime);

    static glm::vec3 OrbitAround(const glm::vec3& direction, const glm::vec3& target, float distance) {
        return target + (direction * distance);
    }

private:
    WindowContext* m_context;
    GLFWwindow* m_window;

    float m_aspectRatio;
    double m_lastMouseX, m_lastMouseY = 0.0;

    const glm::vec3 m_xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
    const glm::vec3 m_yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::vec3 m_zAxis = glm::vec3(0.0f, 0.0f, 1.0f);


    glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_right = glm::vec3(1.0f, 0.0f, 0.0f);

    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewMatrix;

    float m_prevMouseX = 0.0f;
    float m_prevMouseY = 0.0f;
    
    //CameraMode m_mode = CameraMode::PAN;
    CameraMode m_mode = CameraMode::ROTATE;

    void update_projection_matrix();
    void update_view_matrix();
};