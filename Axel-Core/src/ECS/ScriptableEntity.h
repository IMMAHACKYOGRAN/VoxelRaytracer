#pragma once

#include "ECS/Entity.h"

namespace Axel {

	class ScriptableEntity
	{
	public:
		ScriptableEntity() = default;
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent() { return m_Entity.GetComponent<T>(); }

		void SetEntity(Entity e) { m_Entity = e; }
	protected:
		virtual void Start() {}
		virtual void Update(float dt) {}

	private:

		Entity m_Entity;

		friend class Scene;
	};

}
