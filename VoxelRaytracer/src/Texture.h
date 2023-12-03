#pragma once
#include <memory>
#include <string>

class Texture 
{
public:
	virtual ~Texture() = default;

	virtual uint32_t GetWidth() const = 0;
	virtual uint32_t GetHeight() const = 0;

	virtual void Bind() const = 0;
};

class Texture3D : public Texture
{
public:
	Texture3D(const std::string& filePath);
	virtual ~Texture3D();
	static std::shared_ptr<Texture3D> Create(const std::string& filePath);

	virtual uint32_t GetWidth() const override { return m_Width; }
	virtual uint32_t GetHeight() const override { return m_Height; }

	virtual void Bind() const override;

private:
	std::string m_FilePath;

	uint32_t m_Width, m_Height;
	uint32_t m_RendererID;
};