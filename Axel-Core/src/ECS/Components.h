#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/quaternion.hpp>
#include "Renderer/Camera.h"
#include "Renderer/PBRMaterial.h"
#include "Renderer/BasicMesh.h"
#include "Renderer/VoxelModelLoader.h"

namespace Axel
{
	struct NameComponent
	{
		std::string Name = std::string();

		NameComponent() = default;

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

	class ScriptableEntity;

	struct ScriptComponent
	{
		ScriptableEntity* Script = nullptr;

		ScriptComponent() = default;
	};
	
	struct CameraComponent
	{
		Camera Cam;

		CameraComponent() = default;
	};

	struct MeshRendererComponent
	{
		std::string FilePath = std::string();

		PBRMaterial Material;
		BasicMesh Mesh;

		MeshRendererComponent() = default;

		void LoadMesh(const std::string& filePath)
		{
			if (Mesh.LoadMesh(filePath))
				AX_INFO("Loaded: {0}", filePath);
			FilePath = filePath;
		}
	};

	struct PointLightComponent
	{
		glm::vec3 Colour = { 1.0f, 1.0f, 1.0f };
		float Intensity = 10.0f;
	};

	struct VoxelRendererComponent
	{
		BasicVoxelData VoxelData;

		void LoadVoxels(const std::string& path)
		{
			VoxelData = VoxelModelLoader::LoadModelFromFile(path);
		}
	};
}