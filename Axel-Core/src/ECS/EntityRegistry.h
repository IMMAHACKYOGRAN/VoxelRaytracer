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
			m_Pool.at(entity) = component;
			return m_Pool.at(entity);
		}

		void Remove(EntityId entity)
		{
			m_Pool.at(entity) = T();
		}

		T& Get(EntityId entity)
		{
			return m_Pool.at(entity);
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
			m_ComponentToEntities.insert({ componentid, std::vector<EntityId>() });
			if (m_SignitureIndex.insert({ componentid, m_CurrentSignitureIndex }).second)
				m_CurrentSignitureIndex++;
		}

		EntityId CreateEntity() { return GetUniqueID(); }

		void RemoveEntity(EntityId entity)
		{
			// WARNING: Removing this will leave hanging data but it should never be accessed.
			/*for (auto it = m_ComponentPools.begin(); it != m_ComponentPools.end(); it++)
			{
				ComponentPool<decltype(it->second.get())>* componentpool = dynamic_cast<ComponentPool<decltype(it->second.get())>*>(m_ComponentPools[it->first].get());
				componentpool->Remove(entity);
			}*/

			for (auto it = m_ComponentToEntities.begin(); it != m_ComponentToEntities.end(); it++)
				it->second.erase(std::find(it->second.begin(), it->second.end(), entity));

			m_AvailableEntityIDs.push(entity);
			m_Signitures[entity] = ComponentSigniture();
		}

		template<typename T>
		T& AddComponent(EntityId entity)
		{
			T component{};
			ComponentId componentid = typeid(T).hash_code();

			if (m_ComponentPools.find(componentid) == m_ComponentPools.end())
				RegisterComponent<T>();

			AX_ASSERT(!m_Signitures[entity].test(m_SignitureIndex[componentid]), "Entity already has that component");

			ComponentPool<T>* componentpool = dynamic_cast<ComponentPool<T>*>(m_ComponentPools[componentid].get());

			m_Signitures[entity].set(m_SignitureIndex[componentid], true);
			m_ComponentToEntities[componentid].push_back(entity);
			
			return componentpool->Add(entity, component);
		}

		template<typename T>
		void RemoveComponent(EntityId entity)
		{
			ComponentId componentid = typeid(T).hash_code();
			
			if (m_ComponentPools.find(componentid) == m_ComponentPools.end())
				RegisterComponent<T>();

			if (m_Signitures[entity].test(m_SignitureIndex[componentid]))
			{
				ComponentPool<T>* componentpool = dynamic_cast<ComponentPool<T>*>(m_ComponentPools[componentid].get());
				componentpool->Remove(entity);
				m_Signitures[entity].set(m_SignitureIndex[componentid], 0);
				m_ComponentToEntities[componentid].erase(std::find(m_ComponentToEntities[componentid].begin(), m_ComponentToEntities[componentid].end(), entity));
			}
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

			AX_ASSERT(m_Signitures[entity].test(m_SignitureIndex[componentid]), "Attempted to get non existant component");

			ComponentPool<T>* componentpool = dynamic_cast<ComponentPool<T>*>(m_ComponentPools[componentid].get());

			return componentpool->Get(entity);
		}

		template<typename T>
		std::vector<EntityId> GetEntitiesWith()
		{
			ComponentId componentid = typeid(T).hash_code();
			return m_ComponentToEntities[componentid];
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

		std::unordered_map<ComponentId, std::vector<EntityId>> m_ComponentToEntities;

		std::queue<EntityId> m_AvailableEntityIDs;
		uint32_t m_MaxEntities;
	};
} // Reference: https://austinmorlan.com/posts/entity_component_system/