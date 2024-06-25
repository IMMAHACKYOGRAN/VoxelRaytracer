#pragma once
#include <pch.h>

#include "ECS/Components.h"
#include "Scene/Scene.h"

namespace Axel
{
	class Entity
	{
	public:
		Entity() = default;

		Entity(Scene* scene, EntityId id) : m_Scene(scene), m_EntityId(id)
		{
		}

		template<typename T>
		inline T& AddComponent()
		{
			return m_Scene->AddComponent<T>(m_EntityId);
		}

		template<typename T>
		inline bool HasComponent()
		{
			return m_Scene->HasComponent<T>(m_EntityId);
		}

		template<typename T>
		inline T& GetComponent()
		{
			return m_Scene->GetComponent<T>(m_EntityId);
		}

		template<typename T>
		inline void RemoveComponent()
		{
			m_Scene->RemoveComponent<T>(m_EntityId);
		}

		std::string GetName()
		{
			return GetComponent<NameComponent>().Name;
		}

		operator uint32_t() const { return m_EntityId; }
	private:
		EntityId m_EntityId;
		Scene* m_Scene = nullptr;
	};
}