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

	Entity Scene::CreateEntity() { return Entity{ this }; }
}