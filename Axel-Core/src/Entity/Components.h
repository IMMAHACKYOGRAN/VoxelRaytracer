#pragma once
#include <glm.hpp>

struct TransformComponent
{
	glm::vec3 Translation = glm::vec3(0.0f);
	glm::vec3 Rotation = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f);

	TransformComponent() = default;
	TransformComponent(const glm::vec3& translation)
		: Translation(translation) {}
};

struct VoxelRendererComponent
{

};

