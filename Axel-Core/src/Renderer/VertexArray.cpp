#include "pch.h"

#include "Renderer/VertexArray.h"
#include <glad/glad.h>

namespace Axel
{
	static GLenum ShaderDataTypeToOpenGLType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:		return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::Mat3:		return GL_INT;
		case ShaderDataType::Mat4:		return GL_INT;
		case ShaderDataType::Bool:		return GL_BOOL;
		}
	}


	VertexArray::VertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		return std::make_shared<VertexArray>();
	}

	void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t i = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.GetComponentCount(), ShaderDataTypeToOpenGLType(element.Type), element.Normalised ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
			i++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}