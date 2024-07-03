#include "pch.h"

#include "Renderer/BasicMesh.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Axel
{
	bool BasicMesh::LoadMesh(const std::string& filePath)
	{
        const uint32_t flags =
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_SortByPType |
        aiProcess_GenNormals |
        aiProcess_GenUVCoords |
        aiProcess_OptimizeMeshes |
        aiProcess_ValidateDataStructure;

        Assimp::Importer Importer;

        const aiScene* scene = Importer.ReadFile(filePath.c_str(), flags);

        if (scene && scene->HasMeshes())
        {
            auto* mesh = scene->mMeshes[0];

            std::vector<float> meshdata;
            for (uint32_t i = 0; i < mesh->mNumVertices; i++)
            {
                meshdata.push_back(mesh->mVertices[i].x);
                meshdata.push_back(mesh->mVertices[i].y);
                meshdata.push_back(mesh->mVertices[i].z);
                meshdata.push_back(mesh->mNormals[i].x);
                meshdata.push_back(mesh->mNormals[i].y);
                meshdata.push_back(mesh->mNormals[i].z);
            }

            std::shared_ptr<VertexBuffer> vb = VertexBuffer::Create(meshdata.data(), meshdata.size() * sizeof(float));
            vb->SetLayout({
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float3, "a_Normal" },
            });


            std::vector<uint32_t> indexdata;
            for (size_t i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];

                for (size_t j = 0; j < face.mNumIndices; j++)
                    indexdata.push_back(face.mIndices[j]);
            }

            std::shared_ptr<IndexBuffer> ib = IndexBuffer::Create(indexdata.data(), indexdata.size());

            m_VertexArray = VertexArray::Create();
            m_VertexArray->AddVertexBuffer(vb);
            m_VertexArray->SetIndexBuffer(ib);

            return true;
        }

		return false;
	}
}