#include "transform.h"
#include "imgui/imgui_impl_glfw.h"
#include "../resources/fonts/forkawesome-icons.h"

static const float WIDGETWIDTH = 90.0f;
static const float ROTMAX = 360.0f;
static const float ROTMIN = -360.0f;
static const ImVec2 HORIZONTAL = ImVec2(140.0f, 0.0f);
static const ImVec2 BUTTON_SIZE = ImVec2(150, 30);

static const char* HEADER = ICON_FK_CUBE " Transform";


TransformComponent::TransformComponent()
{
	Uuid = generate_uuid_string();
}

TransformComponent::TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
	Position = position;
	Rotation = rotation;
	Scale = scale;
	Uuid = generate_uuid_string();
}

TransformComponent::~TransformComponent()
{
	Parent = nullptr;
}

TransformComponent& TransformComponent::SetPosition(const glm::vec3& position)
{
	Position = position;
	return *this;
}

glm::mat4 TransformComponent::GetModelMatrix() const
{
	const glm::mat4 scale = glm::scale(m_identity, Scale);
	const glm::mat4 rotation = glm::eulerAngleXYZ(
		glm::radians(Rotation.x), 
		glm::radians(Rotation.y), 
		glm::radians(Rotation.z));

	const glm::mat4 translation = translate(m_identity, Position);
	const glm::mat4 model = translation * rotation * scale;

	return Parent ? Parent->GetModelMatrix() * model : model;
}

TransformComponent& TransformComponent::SetPositionX(float pos)
{
	Position = glm::vec3(pos, Position.y, Position.z);
	return *this;
}
TransformComponent& TransformComponent::SetPositionY(float pos)
{
	Position = glm::vec3(Position.x, pos, Position.z);
	return *this;
}
TransformComponent& TransformComponent::SetPositionZ(float pos)
{
	Position = glm::vec3(Position.x, Position.y, pos);
	return *this;
}
//////////////////////////////////
// Rotation
TransformComponent& TransformComponent::SetRotation(const glm::vec3& rotation)
{
	Rotation = rotation;
	return *this;
}
TransformComponent& TransformComponent::SetRotationX(float rot)
{
	Rotation = glm::vec3(rot, Rotation.y, Rotation.z);
	return *this;
}
TransformComponent& TransformComponent::SetRotationY(float rot)
{
	Rotation = glm::vec3(Rotation.x, rot, Rotation.z);
	return *this;
}
TransformComponent& TransformComponent::SetRotationZ(float rot)
{
	Rotation = glm::vec3(Rotation.x, Rotation.y, rot);
	return *this;
}
//////////////////////////////////
// Scale
TransformComponent& TransformComponent::SetScale(const glm::vec3& scale)
{
	Scale = scale;
	return *this;
}
TransformComponent& TransformComponent::SetScaleX(float scale)
{
	Scale = glm::vec3(scale, Scale.y, Scale.z);
	return *this;
}
TransformComponent& TransformComponent::SetScaleY(float scale)
{
	Scale = glm::vec3(Scale.x, scale, Scale.z);
	return *this;
}
TransformComponent& TransformComponent::SetScaleZ(float scale)
{
	Scale = glm::vec3(Scale.x, Scale.y, scale);
	return *this;
}

//////////////////////////////////


TransformComponent& TransformComponent::SetParent(TransformComponent* parent)
{
	Parent = parent; // set pointer to new parent
	Parent->Children.push_back(this); // add child to parent
	return *this;
}

TransformComponent& TransformComponent::AddChild(TransformComponent* child)
{
	Children.push_back(child); // add child to children
	child->Parent = this; // set child's parent
	return *this;
}

// Resets all values to default
TransformComponent& TransformComponent::Reset()
{
    Position = glm::vec3(0.0f);
    Rotation = glm::vec3(0.0f);
    Scale = glm::vec3(1.0f);
    return *this;
}

void TransformComponent::Copy(const TransformComponent& transform)
{
	Position = transform.Position;
	Rotation = transform.Rotation;
	Scale = transform.Scale;
}

void TransformComponent::RotateAround(const glm::vec3& targetPosition, const glm::vec3& axis, float angleInDegrees, float deltaTime)
{
	// Convert the angle to radians
	const float angleInRadians = glm::radians(angleInDegrees);

	// relative position of the object to the target
	const glm::vec3 relativePosition = Position - targetPosition;

	// new pos after rot
	const float x = relativePosition.x * cos(angleInRadians * deltaTime) - relativePosition.z * sin(angleInRadians * deltaTime);
	const float z = relativePosition.x * sin(angleInRadians * deltaTime) + relativePosition.z * cos(angleInRadians * deltaTime);

	// new pos
	Position.x = x + targetPosition.x;
	Position.z = z + targetPosition.z;

	// new rot
	Rotation.y += angleInRadians * deltaTime;
}

void TransformComponent::Translate(const glm::vec3& translation)
{
	Position += translation;
}

void TransformComponent::GuiShowControls(const glm::vec3& resetPos, const glm::vec3& resetRot, const glm::vec3& resetScale)
{
	GuiShowTitle(0.15f, HEADER);
	// Position
	ImGui::Text("Position");
	ImGui::SameLine(HORIZONTAL.x);

	ImGui::Text("X");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(WIDGETWIDTH);
	ImGui::DragFloat(("##PosX" + Uuid).c_str(), &Position.x, 0.1f);
	ImGui::SameLine();
    
	ImGui::Text("Y");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(WIDGETWIDTH);
	ImGui::DragFloat(("##PosY" + Uuid).c_str(), &Position.y, 0.1f);
	ImGui::SameLine();

	ImGui::Text("Z");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(WIDGETWIDTH);
	ImGui::DragFloat(("##PosZ" + Uuid).c_str(), &Position.z, 0.1f);

	// Rotation
	ImGui::Text("Rotation");
	ImGui::SameLine(HORIZONTAL.x);

	ImGui::Text("X");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(WIDGETWIDTH);
	ImGui::DragFloat(("##RotX" + Uuid).c_str(), &Rotation.x, 1.0f, ROTMIN, ROTMAX);
	ImGui::SameLine();

	ImGui::Text("Y");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(WIDGETWIDTH);
	ImGui::DragFloat(("##RotY" + Uuid).c_str(), &Rotation.y, 1.0f, ROTMIN, ROTMAX);
	ImGui::SameLine();

	ImGui::Text("Z");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(WIDGETWIDTH);
	ImGui::DragFloat(("##RotZ" + Uuid).c_str(), &Rotation.z, 1.0f, ROTMIN, ROTMAX);

	// Scale
	ImGui::Text("Scale");
	ImGui::SameLine(HORIZONTAL.x);
	ImGui::Text("X");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(WIDGETWIDTH);
	ImGui::DragFloat(("##ScaleX" + Uuid).c_str(), &Scale.x, 0.1f);
	ImGui::SameLine();

	ImGui::Text("Y");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(WIDGETWIDTH);
	ImGui::DragFloat(("##ScaleY" + Uuid).c_str(), &Scale.y, 0.1f);
	ImGui::SameLine();

	ImGui::Text("Z");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(WIDGETWIDTH);
	ImGui::DragFloat(("##ScaleZ" + Uuid).c_str(), &Scale.z, 0.1f);

	if (ImGui::Button("Reset", BUTTON_SIZE))
	{
		Position = resetPos;
		Rotation = resetRot;
		Scale = resetScale;
	}
}

void TransformComponent::GuiShowControlsExcludeScale(const glm::vec3& resetPos, const glm::vec3& resetRot, const glm::vec3& resetScale)
{
	GuiShowTitle(0.1f, HEADER);
	// Position
	ImGui::Text("Position");
	ImGui::SameLine(HORIZONTAL.x);

	ImGui::Text("X");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(WIDGETWIDTH);
	ImGui::DragFloat(("##PosX" + Uuid).c_str(), &Position.x, 0.1f);
	ImGui::SameLine();
    
	ImGui::Text("Y");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(WIDGETWIDTH);
	ImGui::DragFloat(("##PosY" + Uuid).c_str(), &Position.y, 0.1f);
	ImGui::SameLine();

	ImGui::Text("Z");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(WIDGETWIDTH);
	ImGui::DragFloat(("##PosZ" + Uuid).c_str(), &Position.z, 0.1f);

	// Rotation
	ImGui::Text("Rotation");
	ImGui::SameLine(HORIZONTAL.x);

	ImGui::Text("X");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(WIDGETWIDTH);
	ImGui::DragFloat(("##RotX" + Uuid).c_str(), &Rotation.x, 1.0f, ROTMIN, ROTMAX);
	ImGui::SameLine();

	ImGui::Text("Y");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(WIDGETWIDTH);
	ImGui::DragFloat(("##RotY" + Uuid).c_str(), &Rotation.y, 1.0f, ROTMIN, ROTMAX);
	ImGui::SameLine();

	ImGui::Text("Z");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(WIDGETWIDTH);
	ImGui::DragFloat(("##RotZ" + Uuid).c_str(), &Rotation.z, 1.0f, ROTMIN, ROTMAX);

	if (ImGui::Button("Reset", BUTTON_SIZE))
	{
		Position = resetPos;
		Rotation = resetRot;
		Scale = resetScale;
	}
}

void TransformComponent::GuiShowControlsPosition(const glm::vec3& resetPos, const glm::vec3& resetRot, const glm::vec3& resetScale)
{
	GuiShowTitle(0.1f, HEADER);
	// Position
	ImGui::Text("Position");
	ImGui::SameLine(HORIZONTAL.x);

	ImGui::Text("X");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(WIDGETWIDTH);
	ImGui::DragFloat(("##PosX" + Uuid).c_str(), &Position.x, 0.1f);
	ImGui::SameLine();
    
	ImGui::Text("Y");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(WIDGETWIDTH);
	ImGui::DragFloat(("##PosY" + Uuid).c_str(), &Position.y, 0.1f);
	ImGui::SameLine();

	ImGui::Text("Z");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(WIDGETWIDTH);
	ImGui::DragFloat(("##PosZ" + Uuid).c_str(), &Position.z, 0.1f);

	if (ImGui::Button("Reset", BUTTON_SIZE))
	{
		Position = resetPos;
		Rotation = resetRot;
		Scale = resetScale;
	}
}

