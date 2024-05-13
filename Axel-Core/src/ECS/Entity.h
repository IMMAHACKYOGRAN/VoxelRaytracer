#pragma once
#include <vector>

#include "ECS/Components.h"
#include "Scene/Scene.h"

namespace Axel
{
	class Entity
	{
	public:
		Entity(Scene* scene) : m_Scene(scene) {}

		template<typename T, typename... Args>
		inline T& AddComponent(Args&&... args)
		{
			if(HasComponent<T>()) {}
			T& component = m_Scene->m_Registry.AddComponent<T>(m_EntityID, std::forward<args>...);
			return component;
		}

		template<typename T>
		inline bool HasComponent()
		{
			return m_Scene->m_Registry.HasComponent<T>(m_EntityID);
		}

		operator uint32_t() const { return m_EntityID; }

		bool operator == (const Entity& other) const
		{
			return m_EntityID == other.m_EntityID && m_Scene == other.m_Scene;
		}

		bool operator != (const Entity& other) const
		{
			return !(*this == other);
		}

	private:
		uint32_t m_EntityID;
		Scene* m_Scene = nullptr;
	};
}