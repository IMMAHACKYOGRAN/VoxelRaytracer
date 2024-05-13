#pragma once
#include "ECS/Registry.h"

namespace Axel
{
	// Forward declaration needed in 
	// order to avoid cyclical includes
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity();
		void DestroyEntity(Entity entity);


	private:
		Registry m_Registry;
	};
}