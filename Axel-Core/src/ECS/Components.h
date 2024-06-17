#pragma once
#include <variant>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/quaternion.hpp>

namespace Axel
{
	struct NameComponent
	{
		std::string Name = std::string();

		NameComponent() = default;
		NameComponent(const std::string& name)
			: Name(name) {}

	};

	struct TransformComponent
	{
		glm::vec3 Translation = glm::vec3(0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

		TransformComponent() = default;

		glm::mat4 GetTransform() const
		{
			return glm::translate(glm::mat4(1.0f), Translation)
				 * glm::toMat4(glm::quat(Rotation))
				 * glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct VoxelRendererComponent
	{

	};
}