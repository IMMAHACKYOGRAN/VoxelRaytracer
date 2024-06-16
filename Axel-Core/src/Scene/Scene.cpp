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

	Entity& Scene::CreateEntity()
	{
		Entity e { this, m_Registry.CreateEntity() };
		return e;
	}
}