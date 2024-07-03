#include "pch.h"
#include "VoxelModelLoader.h"

void VoxelModelLoader::ReadInt(std::ifstream& file, int& value)
{
    file.read((char*)&value, sizeof(int));
}

void VoxelModelLoader::ReadRGBA(std::ifstream& file, RGBA& rgba)
{
    file.read((char*)&rgba.r, sizeof(char));
    file.read((char*)&rgba.g, sizeof(char));
    file.read((char*)&rgba.b, sizeof(char));
    file.read((char*)&rgba.a, sizeof(char));
}

void VoxelModelLoader::ReadString(std::ifstream& file, std::string& str, int length)
{
    str.resize(length);
    file.read(&str[0], sizeof(char) * length);
}

void VoxelModelLoader::ReadChunk(std::ifstream& file, Chunk& chunk)
{
    ReadString(file, chunk.Id, 4);
    ReadInt(file, chunk.ChunkSize);
    ReadInt(file, chunk.ChiderenSize);

    chunk.ChunkEnd = (int)file.tellg() + chunk.ChunkSize + chunk.ChiderenSize;
}

BasicVoxelData VoxelModelLoader::LoadModelFromFile(const std::string& filePath)
{
    // Create output object
    BasicVoxelData bvdo;

    // Open file
    std::ifstream in(filePath, std::fstream::in | std::ifstream::binary);
    if (!in)
        return bvdo;

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
            ReadInt(in, bvdo.Size.x);
            ReadInt(in, bvdo.Size.y);
            ReadInt(in, bvdo.Size.z);
        }
        else if (child.Id == "XYZI")
        {
            int voxelcount = 0;
            ReadInt(in, voxelcount);

            if (voxelcount < 0)
                return bvdo;

            int modelsize = bvdo.Size.x * bvdo.Size.y * bvdo.Size.z;
            bvdo.VoxelData.reserve(modelsize);

            for (int i = 0; i < voxelcount; i++)
            {
                uint8_t x, y, z, index;
                in.read((char*)&x, sizeof(char));
                in.read((char*)&y, sizeof(char));
                in.read((char*)&z, sizeof(char));
                in.read((char*)&index, sizeof(char));

                // Collapses 3D data down to single dimentional array ordered x, y, z.
                bvdo.VoxelData[(y * bvdo.Size.z + x) + z * bvdo.Size.x * bvdo.Size.y] = index;
            }
        }
        else if (child.Id == "RGBA")
        {
            for (int i = 0; i < 255; i++)
            {
                ReadRGBA(in, bvdo.Pallete[i]);
            }
        }
    }

    return bvdo;
}
