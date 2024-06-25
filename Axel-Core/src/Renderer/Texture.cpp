#include "pch.h"

#include "Texture.h"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <STBImage/stb_image.h>

namespace Axel
{
	Texture2D::Texture2D(const std::string& filePath)
	{
		stbi_set_flip_vertically_on_load(1);
		int width, height, nrChannels;
		unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

		m_Width = width;
		m_Height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, GL_RGBA8, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	Texture2D::~Texture2D()
	{
	}

	uint8_t* Texture2D::LoadTexture(std::string filePath, int* width, int* height)
	{
		int c;
		uint8_t* data = stbi_load(filePath.c_str(), width, height, &c, 0);
		return data;
	}
}