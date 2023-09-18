#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/euler_angles.hpp>

// glm::vec3(0.0f) shorthand for glm::vec3(0.0f, 0.0f, 0.0f)

// N.B Rotation is stored euler angles for ease of use
class TransformComponent
{
public:
	glm::vec3 Position = glm::vec3(0.0f); 
	glm::vec3 Rotation = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f);

	TransformComponent(){};
	TransformComponent(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	
	// Returns the calculated model matrix
	glm::mat4 GetModelMatrix() const;

	TransformComponent& SetPosition(glm::vec3 position);
	TransformComponent& SetRotation(glm::vec3 rotation);
	TransformComponent& SetScale(glm::vec3 scale);

	// Copies values from another TransformComponent
	void Set(const TransformComponent transform) {
		Position = transform.Position;
		Rotation = transform.Rotation;
		Scale = transform.Scale;
	}

	// Resets all values to default
	TransformComponent& Reset() {
		Position = glm::vec3(0.0f); 
		Rotation = glm::vec3(0.0f); 
		Scale = glm::vec3(1.0f);
		return *this;
	}

	bool operator!=(const TransformComponent& other) const {
		return (Position != other.Position) ||
			(Rotation != other.Rotation) ||
			(Scale != other.Scale);
	}

	bool operator==(const TransformComponent& other) const {
		return (Position == other.Position) &&
			(Rotation == other.Rotation) &&
			(Scale == other.Scale);
	}


private:
	glm::mat4 m_identity = glm::mat4(1.0f); // identity matrix
};

