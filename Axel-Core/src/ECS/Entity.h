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
			T& component = m_Scene->AddComponent<T>(m_EntityId);
			return component;
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

		operator uint32_t() const { return m_EntityId; }

		bool operator == (const Entity& other) const
		{
			return m_EntityId == other.m_EntityId && m_Scene == other.m_Scene;
		}

		bool operator != (const Entity& other) const
		{
			return !(*this == other);
		}

	private:
		EntityId m_EntityId;
		Scene* m_Scene = nullptr;
	};
}