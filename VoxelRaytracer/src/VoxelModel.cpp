#include "VoxelModel.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>

//TODO:
//Remove all openGL code and create external system.

VoxelModel::VoxelModel(const std::string& filePath)
{
    if (!LoadModelFromFile(filePath)) 
    {
        std::cout << "Failed to load: " << filePath << "\n";
        return;
    }

    //not sending data properly

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_3D, m_RendererID);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage3D(GL_TEXTURE_3D, 0, GL_R16UI, m_SizeX, m_SizeY, m_SizeZ, 0, GL_RED_INTEGER, GL_UNSIGNED_SHORT, m_Voxels);

    // Pack ubyte buffer into a uint buffer to send to the GPU.
    /*uint32_t voxelBufferSize = m_VoxelCount / 4;
    uint32_t* dataBuffer = new uint32_t[voxelBufferSize];

    for (uint32_t i = 0; i < 256; i++)
    {
        dataBuffer[i] = 0;
        dataBuffer[i] += (uint32_t)(m_Pallete[i].r);
        dataBuffer[i] += (uint32_t)(m_Pallete[i].g << 8);
        dataBuffer[i] += (uint32_t)(m_Pallete[i].b << 16);
        dataBuffer[i] += (uint32_t)(m_Pallete[i].a << 24);
    }

    for (uint32_t i = 0; i < voxelBufferSize; i++)
    {  
        dataBuffer[i] = 0;
        dataBuffer[i] += (uint32_t)(m_Voxels[(i) * 4]);
        dataBuffer[i] += (uint32_t)(m_Voxels[(i) * 4 + 1] << 8);
        dataBuffer[i] += (uint32_t)(m_Voxels[(i) * 4 + 2] << 16);
        dataBuffer[i] += (uint32_t)(m_Voxels[(i) * 4 + 3] << 24);
    }
 
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, voxelBufferSize, dataBuffer, GL_DYNAMIC_READ);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_RendererID);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    delete[] dataBuffer;*/
}

VoxelModel::~VoxelModel()
{
    delete[] m_Voxels;
    //glDeleteBuffers(1, &m_RendererID);
    glDeleteTextures(1, &m_RendererID);
}

std::shared_ptr<VoxelModel> VoxelModel::Create(const std::string& filePath)
{
	return std::make_shared<VoxelModel>(filePath);
}

void VoxelModel::Bind()
{
    //glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
    glBindTexture(GL_TEXTURE_3D, m_RendererID);
}

void VoxelModel::ReadInt(std::ifstream& file, int& value)
{
    file.read((char*)&value, sizeof(int));
}

void VoxelModel::ReadRGBA(std::ifstream& file, RGBA& rgba)
{
    file.read((char*)&rgba.r, sizeof(char));
    file.read((char*)&rgba.g, sizeof(char));
    file.read((char*)&rgba.b, sizeof(char));
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
            ReadInt(in, m_VoxelCount);

            if (m_VoxelCount < 0)
                return false;

            int modelsize = m_SizeX * m_SizeY * m_SizeZ;
            m_Voxels = new uint8_t[modelsize];
            memset(m_Voxels, 0, sizeof(char) * modelsize);

            for (int i = 0; i < m_VoxelCount; i++)
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
            for (int i = 0; i < 256; i++)
            {
                ReadRGBA(in, m_Pallete[i]);
            }
        }
    }

    return true;
}