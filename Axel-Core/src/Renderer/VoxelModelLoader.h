#pragma once
#include <vector>
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

struct BasicVoxelData
{
	glm::ivec3 Size = glm::ivec3(0);
	RGBA Pallete[256] = {0};
	std::vector<uint8_t> VoxelData;
};

class VoxelModelLoader
{
public:
	static BasicVoxelData LoadModelFromFile(const std::string& filePath);

private:
	static void ReadInt(std::ifstream& file, int& value);
	static void ReadRGBA(std::ifstream& file, RGBA& rgba);
	static void ReadString(std::ifstream& file, std::string& str, int length);
	static void ReadChunk(std::ifstream& file, Chunk& chunk);
};