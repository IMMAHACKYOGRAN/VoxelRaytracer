#include "pch.h"
#include "Scene/Scene.h"
#include "ECS/Entity.h"
#include "ECS/ScriptableEntity.h"

namespace Axel
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity()
	{
		EntityId id = m_Registry.CreateEntity();
		Entity e{ this, id };

		e.AddComponent<TransformComponent>();

		std::string name = "Entity " + std::to_string(id);
		auto& nc = e.AddComponent<NameComponent>();
		nc.Name = name;

		m_EntityMap[id] = e;

		return e;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		EntityId id = m_Registry.CreateEntity();
		Entity e{ this, id };
		e.AddComponent<TransformComponent>();
		auto& nc = e.AddComponent<NameComponent>();
		nc.Name = name;

		m_EntityMap[id] = e;

		return e;
	}

	void Scene::RemoveEntity(EntityId entity)
	{
		if (entity == m_MainCameraEntity)
			m_MainCameraSet = false;

		m_Registry.RemoveEntity(entity);
		m_EntityMap.erase(entity);
	}

	void Scene::OnEditorUpdate(float dt, const OrbitalCamera& camera)
	{
		PointLightComponent light;
		light.Intensity = 0.0f;
		TransformComponent tc;
		const auto lights = m_Registry.GetEntitiesWith<PointLightComponent>();
		if (!lights.empty())
		{
			light = m_Registry.GetComponent<PointLightComponent>(lights.at(0));
			tc = m_Registry.GetComponent<TransformComponent>(lights.at(0));
		}

		Renderer::BeginScene(camera, light, tc);

		for (const auto entity : GetEntitiesWith<MeshRendererComponent>())
			if (!GetComponent<MeshRendererComponent>(entity).FilePath.empty())
				Renderer::DrawMesh(GetComponent<MeshRendererComponent>(entity), GetComponent<TransformComponent>(entity));

		Renderer::EndScene();
	}

	void Scene::OnEditorPlayStart()
	{
		for (const auto e : m_Registry.GetEntitiesWith<ScriptComponent>())
		{
			const auto& s = m_Registry.GetComponent<ScriptComponent>(e).Script;
			if (s != nullptr)
				s->Start();
		}
	}

	void Scene::OnEditorPlayUpdate(float dt)
	{
		for (const auto e : m_Registry.GetEntitiesWith<ScriptComponent>())
		{
			const auto& s = m_Registry.GetComponent<ScriptComponent>(e).Script;
			if (s != nullptr)
				s->Update(dt);
		}

		PointLightComponent light;
		light.Intensity = 0.0f;
		TransformComponent tc;
		const auto lights = m_Registry.GetEntitiesWith<PointLightComponent>();
		if (!lights.empty())
		{
			light = m_Registry.GetComponent<PointLightComponent>(lights.at(0));
			tc = m_Registry.GetComponent<TransformComponent>(lights.at(0));
		}

		if (m_MainCameraSet)
		{
			const auto& cam = m_Registry.GetComponent<CameraComponent>(m_MainCameraEntity).Cam;
			const auto& transform = m_Registry.GetComponent<TransformComponent>(m_MainCameraEntity);

			Renderer::BeginScene(cam, transform.GetTransform(), light, tc);

			for (const auto entity : GetEntitiesWith<MeshRendererComponent>())
				Renderer::DrawMesh(GetComponent<MeshRendererComponent>(entity), GetComponent<TransformComponent>(entity));

			Renderer::EndScene();
		}
	}
	Entity Scene::GetEntityById(EntityId entity)
	{
		return m_EntityMap[entity];
	}
}