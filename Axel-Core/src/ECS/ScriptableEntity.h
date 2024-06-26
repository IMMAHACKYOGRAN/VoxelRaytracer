#pragma once

#pragma once

#include "ECS/Entity.h"

namespace Axel {

	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent() { return m_Entity.GetComponent<T>(); }

	protected:
		virtual void Start() {}
		virtual void Update(float dt) {}

	private:
		Entity m_Entity;

		friend class Scene;
	};

}
