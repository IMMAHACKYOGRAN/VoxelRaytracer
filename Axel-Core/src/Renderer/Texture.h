#pragma once
#include <string>

namespace Axel
{
	class Texture2D
	{
	public:
		Texture2D(const std::string& filePath);
		~Texture2D();

		static uint8_t* LoadTexture(std::string filename, int* width, int* height);

		const uint32_t GetWidth() const { return m_Width; }
		const uint32_t GetHeight() const { return m_Height; }
		const uint32_t GetRendererID() const { return m_RendererID; }

	private:
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
	};
}