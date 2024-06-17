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
}