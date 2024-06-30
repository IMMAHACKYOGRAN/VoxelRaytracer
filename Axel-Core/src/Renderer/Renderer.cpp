#include "pch.h"

#include "Renderer/Renderer.h"

#include <glad/glad.h>
#include <gtc/matrix_transform.hpp>

namespace Axel
{
	struct RendererData
	{
		glm::vec2 ViewportSize;

		std::shared_ptr<VertexArray> CubeVertexArray;
		std::shared_ptr<Shader> VoxelShader;

		glm::mat4 View;
		glm::mat4 Projection;

		Renderer::Stats RenderStats;
	};

	static RendererData s_Data;

	void Renderer::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);

		s_Data.CubeVertexArray = VertexArray::Create();

		float cubeVertices[24] = {
			-0.5f, -0.5f,  0.5f, //   6---7
			 0.5f, -0.5f,  0.5f, //  /|  /|
			-0.5f,  0.5f,  0.5f, // 2---3 |
			 0.5f,  0.5f,  0.5f, // | 4-|-5
			-0.5f, -0.5f, -0.5f, // |/  |/
			 0.5f, -0.5f, -0.5f, // 0---1
			-0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
		};

		std::shared_ptr<VertexBuffer> cubeVertexBuffer = VertexBuffer::Create(cubeVertices, sizeof(cubeVertices));
		cubeVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
		});

		s_Data.CubeVertexArray->AddVertexBuffer(cubeVertexBuffer);

		uint32_t cubeIndecies[36] = {
			0, 3, 2,
			0, 1, 3,
			1, 7, 3,
			1, 5, 7,
			5, 6, 7,
			5, 4, 6,
			4, 2, 6,
			4, 0, 2,
			4, 1, 0,
			4, 5, 1,
			2, 7, 6,
			2, 3, 7,
		};

		std::shared_ptr<IndexBuffer> indexBuffer = IndexBuffer::Create(cubeIndecies, sizeof(cubeIndecies) / sizeof(uint32_t));
		s_Data.CubeVertexArray->SetIndexBuffer(indexBuffer);

		//s_Data.VoxelShader = Shader::Create("res/shaders/3D.glsl");
		s_Data.VoxelShader = Shader::Create("res/shaders/VoxelVert.glsl", "res/shaders/VoxelFrag.glsl");
		s_Data.VoxelShader->Bind();
	}

	void Renderer::Shutdown()
	{
	}

	void Renderer::BeginScene(const OrbitalCamera& camera)
	{
		s_Data.View = camera.GetViewMatrix();
		s_Data.Projection = camera.GetProjectionMatrix();

		s_Data.RenderStats.DrawCalls = 0;
	}

	void Renderer::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		s_Data.View = glm::inverse(transform);
		s_Data.Projection = camera.GetProjectionMatrix();

		s_Data.RenderStats.DrawCalls = 0;
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::DrawCube(const TransformComponent& transform)
	{
		s_Data.VoxelShader->Bind();
		s_Data.VoxelShader->UploadUniformMat4("u_View", s_Data.View);
		s_Data.VoxelShader->UploadUniformMat4("u_Projection", s_Data.Projection);
		s_Data.VoxelShader->UploadUniformMat4("u_Model", transform.GetTransform());

		s_Data.VoxelShader->UploadUniformFloat2("u_ViewportSize", s_Data.ViewportSize);
		
		DrawIndexed();
	}

	void Renderer::DrawIndexed()
	{
		glDrawElements(GL_TRIANGLES, s_Data.CubeVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		s_Data.RenderStats.DrawCalls++;
	}

	void Renderer::Clear(const glm::vec4& colour)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(colour.r, colour.g, colour.b, colour.a);
	}

	void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		s_Data.ViewportSize = { width, height };
		glViewport(x, y, width, height);
	}

	Renderer::Stats Renderer::GetStats()
	{
		return s_Data.RenderStats;
	}

	void Renderer::ClearStats()
	{
		memset(&s_Data.RenderStats, 0, sizeof(Stats));
	}
}