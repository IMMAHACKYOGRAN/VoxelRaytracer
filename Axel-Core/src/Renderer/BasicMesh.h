#pragma once
#include "Renderer/VertexArray.h"

namespace Axel
{
	class BasicMesh
	{
	public:
		bool LoadMesh(const std::string& filePath);

		std::shared_ptr<VertexArray> GetVertexArray() const { return m_VertexArray; }

	private:
		std::shared_ptr<VertexArray> m_VertexArray;
	};
}