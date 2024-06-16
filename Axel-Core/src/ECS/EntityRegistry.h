#pragma once
#include "pch.h"
#include "ECS/Components.h"

namespace Axel
{
	using EntityId = uint32_t;
	using ComponentSigniture = std::bitset<32>;
	using ComponentId = decltype(std::declval<std::type_info>().hash_code());

	class BaseComponentPool
	{
	public:
		virtual ~BaseComponentPool() {}
	};

	template <typename T>
	class ComponentPool : public BaseComponentPool
	{
	public:
		ComponentPool(uint32_t size) : m_Size(size)
		{
			m_Pool.reserve(size);

			for (uint32_t i = 0; i < size; i++)
				m_Pool.push_back(T());
		}

		T& Add(EntityId entity, T& component)
		{
			m_Pool[entity] = component;
			return m_Pool[entity];
		}

		T& Get(EntityId entity)
		{
			return m_Pool[entity];
		}

	private:
		uint32_t m_Size;
		std::vector<T> m_Pool;
	};

	class EntityRegistry
	{
	public:
		EntityRegistry(uint32_t maxEntities = 1000) : m_MaxEntities(maxEntities)
		{
			m_Signitures.reserve(m_MaxEntities);

			for (EntityId id = 0; id < m_MaxEntities; id++)
			{
				m_AvailableEntityIDs.push(id);
				m_Signitures.push_back(ComponentSigniture());
			}
		}

		template<typename T>
		void RegisterComponent()
		{
			ComponentId componentid = typeid(T).hash_code();
			m_ComponentPools.insert({ componentid, std::make_unique<ComponentPool<T>>(m_MaxEntities) });
			if (m_SignitureIndex.insert({ componentid, m_CurrentSignitureIndex }).second)
				m_CurrentSignitureIndex++;
		}

		EntityId CreateEntity() { return GetUniqueID(); }

		template<typename T>
		T& AddComponent(EntityId entity)
		{
			T component{};
			ComponentId componentid = typeid(T).hash_code();

			if (m_ComponentPools.find(componentid) == m_ComponentPools.end())
				RegisterComponent<T>();

			ComponentPool<T>* componentpool = dynamic_cast<ComponentPool<T>*>(m_ComponentPools[componentid].get());

			componentpool->Add(entity, component);
			m_Signitures[entity].set(m_SignitureIndex[componentid], true);
			
			return component;
		}

		template<typename T>
		bool HasComponent(EntityId entity)
		{
			ComponentId componentid = typeid(T).hash_code();
			
			if (m_ComponentPools.find(componentid) == m_ComponentPools.end())
				RegisterComponent<T>();

			return m_Signitures[entity].test(m_SignitureIndex[componentid]);
		}

		template<typename T>
		T& GetComponent(EntityId entity)
		{
			ComponentId componentid = typeid(T).hash_code();

			if (m_ComponentPools.find(componentid) == m_ComponentPools.end())
				RegisterComponent<T>();

			ComponentPool<T>* componentpool = dynamic_cast<ComponentPool<T>*>(m_ComponentPools[componentid].get());

			return componentpool->Get(entity);
		}

	private:
		EntityId GetUniqueID()
		{
			EntityId id = m_AvailableEntityIDs.front();
			m_AvailableEntityIDs.pop();
			return id;
		}

	private:
		std::unordered_map<ComponentId, std::unique_ptr<BaseComponentPool>> m_ComponentPools;

		uint32_t m_CurrentSignitureIndex = 0;
		std::unordered_map<ComponentId, uint32_t> m_SignitureIndex;
		std::vector<ComponentSigniture> m_Signitures;

		std::queue<EntityId> m_AvailableEntityIDs;
		uint32_t m_MaxEntities;
	};
} // Reference: https://austinmorlan.com/posts/entity_component_system/