#pragma once

#include "Scene/Scene.h"

namespace Axel
{
	class Serializer
	{
	public:
		Serializer(const std::shared_ptr<Scene>& scene);

		void Serialize(const std::string& filepath, const OrbitalCamera& camera);
		bool Deserialize(const std::string& filepath, OrbitalCamera& camera);

	private:
		std::shared_ptr<Scene> m_Scene;
	};

}