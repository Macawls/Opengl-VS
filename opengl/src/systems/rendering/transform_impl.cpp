#include "transform.h"

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