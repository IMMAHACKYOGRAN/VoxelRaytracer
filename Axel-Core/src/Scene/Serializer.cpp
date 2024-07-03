#include "pch.h"
#include "Serializer.h"
#include "ECS/Entity.h"

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& v)
		{
			Node node;
			node.push_back(v.x);
			node.push_back(v.y);
			node.push_back(v.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& v)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			v.x = node[0].as<float>();
			v.y = node[1].as<float>();
			v.z = node[2].as<float>();
			return true;
		}
	};

}

namespace Axel
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& value)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << value.z << YAML::EndSeq;
		return out;
	}


	Serializer::Serializer(const std::shared_ptr<Scene>& scene)
		: m_Scene(scene)
	{
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << (EntityId)entity;

		if (entity.HasComponent<NameComponent>())
		{
			out << YAML::Key << "NameComponent";
			out << YAML::BeginMap;

			const auto& name = entity.GetComponent<NameComponent>().Name;
			out << YAML::Key << "Name" << YAML::Value << name;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			const auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation"	  << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale"		  << YAML::Value << tc.Scale;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto& c = entity.GetComponent<CameraComponent>().Cam;
			out << YAML::Key << "VerticalFov" << YAML::Value << c.GetFOV();
			out << YAML::Key << "NearClip" << YAML::Value << c.GetNearClip();
			out << YAML::Key << "FarClip" << YAML::Value << c.GetFarClip();

			out << YAML::EndMap;
		}

		if (entity.HasComponent<MeshRendererComponent>())
		{
			out << YAML::Key << "MeshRendererComponent";
			out << YAML::BeginMap;

			auto& m = entity.GetComponent<MeshRendererComponent>();
			out << YAML::Key << "FilePath" << YAML::Value << m.FilePath;

			out << YAML::Value << "Material";
			out << YAML::BeginMap;
			out << YAML::Key << "Albedo" << YAML::Value << m.Material.Albedo;
			out << YAML::Key << "Metalic" << YAML::Value << m.Material.Metalic;
			out << YAML::Key << "Roughness" << YAML::Value << m.Material.Roughness;
			out << YAML::EndMap;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<PointLightComponent>())
		{
			out << YAML::Key << "PointLightComponent";
			out << YAML::BeginMap;

			auto& p = entity.GetComponent<PointLightComponent>();
			out << YAML::Key << "Colour" << YAML::Value << p.Colour;
			out << YAML::Key << "Intensity" << YAML::Value << p.Intensity;

			out << YAML::EndMap;
		}

		out << YAML::EndMap; // Entity
	}

	void Serializer::Serialize(const std::string& filepath, const OrbitalCamera& camera)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";

		out << YAML::Value << "EditorCamera";
		out << YAML::BeginMap;
		out << YAML::Key << "VerticalFov" << YAML::Value << camera.m_VerticalFov;
		out << YAML::Key << "NearClip"    << YAML::Value << camera.m_NearClip;
		out << YAML::Key << "FarClip"	  << YAML::Value << camera.m_FarClip;
		out << YAML::Key << "FocalPoint"  << YAML::Value << camera.m_FocalPoint;
		out << YAML::Key << "Distance"    << YAML::Value << camera.m_Distance;
		out << YAML::Key << "Pitch"		  << YAML::Value << camera.m_Pitch;
		out << YAML::Key << "Yaw"		  << YAML::Value << camera.m_Yaw;
		out << YAML::EndMap;

		out << YAML::Key << "Entities" << YAML::Value;
		out << YAML::BeginSeq;

		for (const auto e : m_Scene->GetEntitiesWith<NameComponent>())
			SerializeEntity(out, m_Scene->GetEntityById(e));

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	bool Serializer::Deserialize(const std::string& filepath, OrbitalCamera& camera)
	{
		std::ifstream stream(filepath);
		std::stringstream ss;

		ss << stream.rdbuf();

		YAML::Node data = YAML::Load(ss.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();

		auto editorcam = data["EditorCamera"];
		float fov = editorcam["VerticalFov"].as<float>();
		float nc = editorcam["NearClip"].as<float>();
		float fc = editorcam["FarClip"].as<float>();
		camera = OrbitalCamera(fov, 45.0f, nc, fc);
		camera.SetFocalPoint(editorcam["FocalPoint"].as<glm::vec3>());
		camera.SetDistance(editorcam["Distance"].as<float>());
		camera.SetPitchAndYaw(editorcam["Pitch"].as<float>(), editorcam["Yaw"].as<float>());

		auto entitylist = data["Entities"];
		for (auto entity : entitylist)
		{
			Entity newEntity = m_Scene->CreateEntity(entity["NameComponent"]["Name"].as<std::string>());
			auto transformComponent = entity["TransformComponent"];
			if (transformComponent)
			{
				// Entities always have transforms
				auto& tc = newEntity.GetComponent<TransformComponent>();
				tc.Translation = transformComponent["Translation"].as<glm::vec3>();
				tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
				tc.Scale = transformComponent["Scale"].as<glm::vec3>();
			}
			auto cameraComponent = entity["CameraComponent"];
			if (cameraComponent)
			{
				auto& cc = newEntity.AddComponent<CameraComponent>();
				cc.Cam.SetFOV(cameraComponent["VerticalFov"].as<float>());
				cc.Cam.SetNearClip(cameraComponent["NearClip"].as<float>());
				cc.Cam.SetFarClip(cameraComponent["FarClip"].as<float>());
				m_Scene->SetMainCameraEntity((EntityId)newEntity);
			}
			auto meshComponent = entity["MeshRendererComponent"];
			if (meshComponent)
			{
				auto& m = newEntity.AddComponent<MeshRendererComponent>();
				m.FilePath = meshComponent["FilePath"].as<std::string>();
				m.Material.Albedo = meshComponent["Material"]["Albedo"].as<glm::vec3>();
				m.Material.Metalic = meshComponent["Material"]["Metalic"].as<float>();
				m.Material.Roughness = meshComponent["Material"]["Roughness"].as<float>();
				m.LoadMesh(m.FilePath);
			}
			auto pointLightComponent = entity["PointLightComponent"];
			if (pointLightComponent)
			{
				auto& pl = newEntity.AddComponent<PointLightComponent>();
				pl.Colour = pointLightComponent["Colour"].as<glm::vec3>();
				pl.Intensity = pointLightComponent["Intensity"].as<float>();
			}
		}

		return false;
	}
}
