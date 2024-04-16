#pragma once
#include <memory>
#include <string>
#include <glm.hpp>

struct FileStructure
{
	std::string Id;
	int Version;
};

struct Chunk
{
	std::string Id;
	int ChunkSize;
	int ChiderenSize;
	size_t ChunkEnd;
};

struct RGBA
{
	uint8_t r, g, b, a;
};

class VoxelModel
{
public:
	VoxelModel(const std::string& filePath);
	~VoxelModel();

	static std::shared_ptr<VoxelModel> Create(const std::string& filePath);

	inline glm::vec3 GetSize() { return { m_SizeX, m_SizeY, m_SizeZ }; }

	void Bind();

private:
	void ReadInt(std::ifstream& file, int& value);
	void ReadRGBA(std::ifstream& file, RGBA& rgba);
	void ReadString(std::ifstream& file, std::string& str, int length);
	void ReadChunk(std::ifstream& file, Chunk& chunk);

	bool LoadModelFromFile(const std::string& filePath);

private:
	uint32_t m_RendererID;

	int m_SizeX, m_SizeY, m_SizeZ;

	uint8_t* m_Voxels; // 3D texture containing pallet index, 0 = no voxel.
	int m_VoxelCount;

	RGBA m_Pallete[256];
};