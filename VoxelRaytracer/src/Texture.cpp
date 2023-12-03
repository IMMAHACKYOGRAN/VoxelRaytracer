#include "Texture.h"

Texture3D::Texture3D(const std::string& filePath)
	: m_FilePath(filePath)
{

}

Texture3D::~Texture3D()
{
}

std::shared_ptr<Texture3D> Texture3D::Create(const std::string& filePath)
{
	return std::shared_ptr<Texture3D>();
}

void Texture3D::Bind() const
{
}