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

		Entity CreateEntity();
		Entity CreateEntity(const std::string& name);
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
		std::vector<EntityId> GetEntitiesWith()
		{
			return m_Registry.GetEntitiesWith<T>();
		}

	private:
		EntityRegistry m_Registry{10};

		bool m_IsRunning = false;
		bool m_IsPaused = false;
	};
}