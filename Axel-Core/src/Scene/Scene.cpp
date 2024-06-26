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
		m_Registry.RemoveEntity(entity);
		m_EntityMap.erase(entity);
	}

	void Scene::OnEditorUpdate(float dt, const OrbitalCamera& camera)
	{
		Renderer::BeginScene(camera);

		for (const auto entity : GetEntitiesWith<TransformComponent>())
			Renderer::DrawCube(GetComponent<TransformComponent>(entity));

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

		const auto& cam = m_Registry.GetComponent<CameraComponent>(m_MainCameraEntity).Cam;
		const auto& transform = m_Registry.GetComponent<TransformComponent>(m_MainCameraEntity);
		
		if (cam != nullptr)
		{
			Renderer::BeginScene(*cam, transform.GetTransform());

			for (const auto entity : GetEntitiesWith<TransformComponent>())
				Renderer::DrawCube(GetComponent<TransformComponent>(entity));

			Renderer::EndScene();
		}
	}
	Entity Scene::GetEntityById(EntityId entity)
	{
		return m_EntityMap[entity];
	}
}