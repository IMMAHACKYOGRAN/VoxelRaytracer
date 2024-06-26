#pragma once

#include "Scene/Scene.h"

namespace Axel
{
	class Serializer
	{
	public:
		Serializer(const std::shared_ptr<Scene>& scene);

		void Serialize(const std::string& filepath);
		bool Deserialize(const std::string& filepath);

	private:
		std::shared_ptr<Scene> m_Scene;
	};

}