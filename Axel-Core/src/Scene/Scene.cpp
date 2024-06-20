#include "pch.h"
#include "Scene/Scene.h"
#include "ECS/Entity.h"

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

		std::string name = "Entity " + std::to_string(id);
		auto& nc = e.AddComponent<NameComponent>();
		nc.Name = name;

		return e;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity e{ this, m_Registry.CreateEntity() };
		auto& nc = e.AddComponent<NameComponent>();
		nc.Name = name;

		return e;
	}

	void Scene::OnEditorStart()
	{
	}

	void Scene::OnEditorUpdate(float dt)
	{
		
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
	} 
}