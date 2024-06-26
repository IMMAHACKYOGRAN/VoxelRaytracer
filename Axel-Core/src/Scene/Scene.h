#pragma once
#include <ECS/EntityRegistry.h>
#include "Renderer/Renderer.h"

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
		void RemoveEntity(EntityId entity);

		void SetMainCameraEntity(EntityId entity) { m_MainCameraEntity = entity; }

		void OnEditorUpdate(float dt, const OrbitalCamera& camera);

		void OnEditorPlayStart();
		void OnEditorPlayUpdate(float dt);

		template<typename T>
		T& AddComponent(EntityId entity) { return m_Registry.AddComponent<T>(entity); }

		template<typename T>
		bool HasComponent(EntityId entity) { return m_Registry.HasComponent<T>(entity); }

		template<typename T>
		T& GetComponent(EntityId entity) { return m_Registry.GetComponent<T>(entity); }

		template<typename T>
		void RemoveComponent(EntityId entity) 
		{
			if (typeid(T) == typeid(NameComponent) || typeid(T) == typeid(TransformComponent))
				return;

			m_Registry.RemoveComponent<T>(entity); 
		}

		template<typename T>
		std::vector<EntityId> GetEntitiesWith()	{ return m_Registry.GetEntitiesWith<T>(); }

		Entity GetEntityById(EntityId entity);

	private:
		EntityRegistry m_Registry{10};
		std::unordered_map<EntityId, Entity> m_EntityMap;

		EntityId m_MainCameraEntity;

		friend class Serializer;
	};
}