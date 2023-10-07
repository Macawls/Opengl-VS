#include "transform.h"
#include "imgui/imgui_impl_glfw.h"
#include "../../utils/gui.h"
#include "../resources/fonts/forkawesome-icons.h"

static const float WIDGETWIDTH = 90.0f;
static const float ROTMAX = 360.0f;
static const float ROTMIN = 0.0f;
static const ImVec2 HORIZONTAL = ImVec2(120.0f, 0.0f);
static const ImVec2 BUTTON_SIZE = ImVec2(150, 30);

static const char* HEADER = ICON_FK_CUBE " Transform";

TransformComponent::TransformComponent(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	Position = position;
	Rotation = rotation;
	Scale = scale;
}

TransformComponent& TransformComponent::SetPosition(glm::vec3 position)
{
	Position = position;
	return *this;
}

TransformComponent& TransformComponent::SetRotation(glm::vec3 rotation)
{
	Rotation = rotation;
	return *this;
}

TransformComponent& TransformComponent::SetScale(glm::vec3 scale)
{
	Scale = scale;
	return *this;
}

glm::mat4 TransformComponent::GetModelMatrix() const
{
	glm::mat4 scale = glm::scale(m_identity, Scale);
	glm::mat4 rotation = glm::eulerAngleXYZ(
		glm::radians(Rotation.x), 
		glm::radians(Rotation.y), 
		glm::radians(Rotation.z));
	
	glm::mat4 translation = glm::translate(m_identity, Position);

	return translation * rotation * scale;
}

void TransformComponent::ShowControls(TransformComponent resetTarget)
{
    ShowTitle(0.1f, HEADER);

    // Position
    ImGui::Text("Position");
    ImGui::SameLine(HORIZONTAL.x);

    ImGui::Text("X");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(WIDGETWIDTH);
    ImGui::DragFloat("##PosX", &Position.x, 0.1f);
    ImGui::SameLine();
    
    ImGui::Text("Y");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(WIDGETWIDTH);
    ImGui::DragFloat("##PosY", &Position.y, 0.1f);
    ImGui::SameLine();

    ImGui::Text("Z");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(WIDGETWIDTH);
    ImGui::DragFloat("##PosZ", &Position.z, 0.1f);

    // Rotation
    ImGui::Text("Rotation");
    ImGui::SameLine(HORIZONTAL.x);

    ImGui::Text("X");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(WIDGETWIDTH);
    ImGui::DragFloat("##RotX", &Rotation.x, 1.0f, ROTMIN, ROTMAX);
    ImGui::SameLine();

    ImGui::Text("Y");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(WIDGETWIDTH);
    ImGui::DragFloat("##RotY", &Rotation.y, 1.0f, ROTMIN, ROTMAX);
    ImGui::SameLine();

    ImGui::Text("Z");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(WIDGETWIDTH);
    ImGui::DragFloat("##RotZ", &Rotation.z, 1.0f, ROTMIN, ROTMAX);

    // Scale
    ImGui::Text("Scale");
    ImGui::SameLine(HORIZONTAL.x);
    ImGui::Text("X");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(WIDGETWIDTH);
    ImGui::DragFloat("##ScaleX", &Scale.x, 0.1f);
    ImGui::SameLine();

    ImGui::Text("Y");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(WIDGETWIDTH);
    ImGui::DragFloat("##ScaleY", &Scale.y, 0.1f);
    ImGui::SameLine();

    ImGui::Text("Z");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(WIDGETWIDTH);
    ImGui::DragFloat("##ScaleZ", &Scale.z, 0.1f);

    if (ImGui::Button("Reset", BUTTON_SIZE))
    {
        Copy(resetTarget);
	}
}

void TransformComponent::ShowControlsExcludeScale(TransformComponent resetTarget)
{
    ShowTitle(0.1f, HEADER);

    // Position
    ImGui::Text("Position");
    ImGui::SameLine(HORIZONTAL.x);

    ImGui::Text("X");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(WIDGETWIDTH);
    ImGui::DragFloat("##PosX", &Position.x, 0.1f);
    ImGui::SameLine();

    ImGui::Text("Y");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(WIDGETWIDTH);
    ImGui::DragFloat("##PosY", &Position.y, 0.1f);
    ImGui::SameLine();

    ImGui::Text("Z");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(WIDGETWIDTH);
    ImGui::DragFloat("##PosZ", &Position.z, 0.1f);

    // Rotation
    ImGui::Text("Rotation");
    ImGui::SameLine(HORIZONTAL.x);

    ImGui::Text("X");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(WIDGETWIDTH);
    ImGui::DragFloat("##RotX", &Rotation.x, 1.0f, ROTMIN, ROTMAX);
    ImGui::SameLine();

    ImGui::Text("Y");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(WIDGETWIDTH);
    ImGui::DragFloat("##RotY", &Rotation.y, 1.0f, ROTMIN, ROTMAX);
    ImGui::SameLine();

    ImGui::Text("Z");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(WIDGETWIDTH);
    ImGui::DragFloat("##RotZ", &Rotation.z, 1.0f, ROTMIN, ROTMAX);

    if (ImGui::Button("Reset", BUTTON_SIZE))
    {
        Copy(resetTarget);
    }
}
