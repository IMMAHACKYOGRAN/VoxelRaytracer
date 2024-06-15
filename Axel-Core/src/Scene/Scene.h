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
		void DestroyEntity(Entity entity);

		template<typename T>
		std::vector<EntityId> GetEntitiesWith()	{ return m_Registry.GetEntitiesWith<T>(); }

		template<typename T>
		T& GetComponentFromEntity(EntityId id) { return m_Registry.GetComponentFromEntity<T>(id); }

		EntityRegistry<TransformComponent, VoxelRendererComponent> m_Registry;
	};
}