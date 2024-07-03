#pragma once
#include <glm.hpp>

namespace Axel
{
	struct PBRMaterial
	{
		glm::vec3 Albedo = glm::vec3(1.0f);
		float Metalic = 0.0f;
		float Roughness = 0.0f;
	};
}