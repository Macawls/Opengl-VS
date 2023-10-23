#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "../../utils/gen_uuid.h"
#include "../../utils/gui.h"

// glm::vec3(0.0f) shorthand for glm::vec3(0.0f, 0.0f, 0.0f)

// N.B Rotation is stored euler angles for ease of use
class TransformComponent
{
public:
	glm::vec3 Position = glm::vec3(0.0f); 
	glm::vec3 Rotation = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f);

	std::string GuiDisplay;
	std::string Uuid;
	TransformComponent* Parent = nullptr;
	std::vector<TransformComponent*> Children;
	
	TransformComponent();
	TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	~TransformComponent();

	// Returns the calculated model matrix
	glm::mat4 GetModelMatrix() const;

	bool HasParent() const
	{
		return Parent;
	}

	TransformComponent& SetPositionX(float pos);
	TransformComponent& SetPositionY(float pos);
	TransformComponent& SetPositionZ(float pos);

	TransformComponent& SetRotationX(float rot);
	TransformComponent& SetRotationY(float rot);
	TransformComponent& SetRotationZ(float rot);

	TransformComponent& SetScaleX(float scale);
	TransformComponent& SetScaleY(float scale);
	TransformComponent& SetScaleZ(float scale);

	TransformComponent& AddChild(TransformComponent* child);
	TransformComponent& SetParent(TransformComponent* parent);
	TransformComponent& SetPosition(const glm::vec3& position);
	TransformComponent& SetRotation(const glm::vec3& rotation);
	TransformComponent& SetScale(const glm::vec3& scale);

	// Copies values from another TransformComponent
	void Copy(const TransformComponent& transform);

	// Resets all values to default
	TransformComponent& Reset();


	bool operator!=(const TransformComponent& other) const {
		return Position != other.Position ||
			Rotation != other.Rotation ||
			Scale != other.Scale;
	}

	bool operator==(const TransformComponent& other) const {
		return Position == other.Position &&
			Rotation == other.Rotation &&
			Scale == other.Scale;
	}

	// Show ImGui controls for this TransformComponent
	void ShowControls(const glm::vec3& resetPos = glm::vec3(0.0f),
		const glm::vec3& resetRot = glm::vec3(0.0f),
		const glm::vec3& resetScale = glm::vec3(1.0f));

	void ShowControlsExcludeScale(const glm::vec3& resetPos = glm::vec3(0.0f),
		const glm::vec3& resetRot = glm::vec3(0.0f),
		const glm::vec3& resetScale = glm::vec3(1.0f));

private:
	inline static glm::mat4 m_identity = glm::mat4(1.0f); // identity matrix
};

