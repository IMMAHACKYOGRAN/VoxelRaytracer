#pragma once
#include "pch.h"
#include "ECS/Components.h"

namespace Axel
{
	using EntityId = std::uint32_t;
	using ComponentId = decltype(std::declval<std::type_info>().hash_code());
	using ComponentNameType = decltype(std::declval<std::type_info>().name());

	template<typename... Components>
	class EntityRegistry
	{
	public:
		EntityRegistry(uint32_t maxEntities = 10000) : m_MaxEntities(maxEntities)
		{
			for (EntityId id = 0; id < m_MaxEntities; id++)
				m_AvailableEntityIDs.push(id);

			init = true;
		}

		EntityId CreateEntity() { return GetUniqueID(); }

		template<typename T, typename... Args>
		T& AddComponent(EntityId entity, Args&&... args)
		{
			T component{};
			ComponentId componentid = typeid(T).hash_code();
			m_ComponentsToEntities[componentid].push_back({ entity, component });
			m_EntitiesToComponents[entity].push_back(component);
			return component;
		}

		// TODO: Make it work for multiple types.
		template<typename T>
		std::vector<EntityId> GetEntitiesWith()
		{
			std::vector<EntityId> ret;
			ComponentId componentid = typeid(T).hash_code();
			for (auto& pair : m_ComponentsToEntities[componentid])
				ret.push_back(std::get<0>(pair));

			return ret;
		}

		template<typename T> // Finish
		T& GetComponentFromEntity(EntityId id)
		{
			m_EntitiesToComponents[id];
			T& ret = std::get<T>();
			return ret;
		}

	private:
		EntityId GetUniqueID()
		{
 			AX_ASSERT(init, "Entity registry not initialised");

			EntityId id = m_AvailableEntityIDs.front();
			m_AvailableEntityIDs.pop();
			return id;

		}

	private:
		std::unordered_map<ComponentId, std::vector<std::pair<EntityId, std::variant<Components...>>>> m_ComponentsToEntities;
		std::unordered_map<EntityId, std::vector<std::variant<Components...>>> m_EntitiesToComponents;
		std::unordered_map<ComponentId, uint32_t> m_VariantMap;

		std::queue<EntityId> m_AvailableEntityIDs;
		uint32_t m_MaxEntities;

		bool init = false;
	};
} // Reference: https://austinmorlan.com/posts/entity_component_system/