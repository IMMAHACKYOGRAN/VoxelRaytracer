#pragma once
#include <ECS/EntityRegistry.h>

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

		Entity& CreateEntity();
		void DestroyEntity(Entity entity);

		template<typename T>
		T& AddComponent(EntityId entity)
		{
			return m_Registry.AddComponent<T>(entity);
		}

		template<typename T>
		bool HasComponent(EntityId entity)
		{
			return m_Registry.HasComponent<T>(entity);
		}

		template<typename T>
		T& GetComponent(EntityId entity)
		{
			return m_Registry.GetComponent<T>(entity);
		}

		template<typename T>
		std::vector<EntityId> GetEntitysWith()
		{

		}

	private:
		EntityRegistry m_Registry;
	};
}