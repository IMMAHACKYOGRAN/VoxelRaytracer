#pragma once
#include <memory>
#include <string>

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
	VoxelModel();
	VoxelModel(const std::string& filePath);
	~VoxelModel();

	static std::shared_ptr<VoxelModel> Create();
	static std::shared_ptr<VoxelModel> Create(const std::string& filePath);

	bool LoadModelFromFile(const std::string& filePath);

	inline int GetSizeX() { return m_SizeX; }
	inline int GetSizeY() { return m_SizeY; }
	inline int GetSizeZ() { return m_SizeZ; }

private:
	void ReadInt(std::ifstream& file, int& value);
	void ReadRGBA(std::ifstream& file, RGBA& rgba);
	void ReadString(std::ifstream& file, std::string& str, int length);
	void ReadChunk(std::ifstream& file, Chunk& chunk);

private:
	int m_SizeX, m_SizeY, m_SizeZ;

	uint8_t* m_Voxels; // 3D texture containing pallet intex, 0 = no voxel.

	RGBA m_Pallete[256];
};