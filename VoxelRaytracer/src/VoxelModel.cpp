#include "VoxelModel.h"

#include<iostream>
#include<fstream>

VoxelModel::VoxelModel()
{
}

VoxelModel::VoxelModel(const std::string& filePath)
{
    if (!LoadModelFromFile(filePath))
        std::cout << "Failed to load: " << filePath << "\n";
}

VoxelModel::~VoxelModel()
{
    delete[] m_Voxels;
}

std::shared_ptr<VoxelModel> VoxelModel::Create(const std::string& filePath)
{
	return std::make_shared<VoxelModel>(filePath);
}

std::shared_ptr<VoxelModel> VoxelModel::Create()
{
    return std::make_shared<VoxelModel>();
}

void VoxelModel::ReadInt(std::ifstream& file, int& value)
{
    file.read((char*)&value, sizeof(int));
}

void VoxelModel::ReadRGBA(std::ifstream& file, RGBA& rgba)
{
    file.read((char*)&rgba.r, sizeof(char));
    file.read((char*)&rgba.b, sizeof(char));
    file.read((char*)&rgba.g, sizeof(char));
    file.read((char*)&rgba.a, sizeof(char));
}

void VoxelModel::ReadString(std::ifstream& file, std::string& str, int length)
{
    str.resize(length);
    file.read(&str[0], sizeof(char) * length);
}

void VoxelModel::ReadChunk(std::ifstream& file, Chunk& chunk)
{
    ReadString(file, chunk.Id, 4);
    ReadInt(file, chunk.ChunkSize);
    ReadInt(file, chunk.ChiderenSize);

    chunk.ChunkEnd = (int)file.tellg() + chunk.ChunkSize + chunk.ChiderenSize;
}

bool VoxelModel::LoadModelFromFile(const std::string& filePath)
{
    // Open file
    std::ifstream in(filePath, std::fstream::in | std::ifstream::binary);
    if (!in)
        return false;

    // Go to start of file
    in.seekg(0, std::ios::beg);

    // Begin reading file
    FileStructure fs;
    ReadString(in, fs.Id, 4);
    ReadInt(in, fs.Version);

    Chunk mainChunk;
    ReadChunk(in, mainChunk);

    // Skip the main chunk because it does not contain data
    in.seekg(mainChunk.ChunkSize, std::ios::cur);

    // Read the contents of child chunks
    while ((int)in.tellg() < mainChunk.ChunkEnd)
    {
        Chunk child;
        ReadChunk(in, child);

        if (child.Id == "SIZE")
        {
            ReadInt(in, m_SizeX);
            ReadInt(in, m_SizeY);
            ReadInt(in, m_SizeZ);
        }
        else if (child.Id == "XYZI")
        {
            int voxelCount;
            ReadInt(in, voxelCount);

            if (voxelCount < 0)
                return false;

            int modelsize = m_SizeX * m_SizeY * m_SizeZ;
            m_Voxels = new uint8_t[modelsize];
            memset(m_Voxels, 0, sizeof(char) * modelsize);

            for (int i = 0; i < voxelCount; i++)
            {
                uint8_t x, y, z, index;
                in.read((char*)&x, sizeof(char));
                in.read((char*)&y, sizeof(char));
                in.read((char*)&z, sizeof(char));
                in.read((char*)&index, sizeof(char));

                // Collapses 3D data down to single dimentional array ordered x, y, z.
                m_Voxels[(y * m_SizeY + x) + z * m_SizeX * m_SizeY] = index;
            }
        }
        else if (child.Id == "RGBA")
        {
            for (int i = 0; i < 255; i++)
            {
                ReadRGBA(in, m_Pallete[i]);
            }
        }
    }

    return true;
}