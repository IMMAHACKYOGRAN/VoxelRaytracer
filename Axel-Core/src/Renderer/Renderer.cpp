#include "pch.h"

#include "Renderer/Renderer.h"

#include <glad/glad.h>
#include <gtc/matrix_transform.hpp>

namespace Axel
{
	struct RendererData
	{
		glm::vec3 CameraPos;

		PointLightComponent Light;
		glm::vec3 LightPos;

		std::shared_ptr<Shader> PBRShader;

		std::shared_ptr<VertexArray> Mesh;

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

		s_Data.PBRShader = Shader::Create("res/shaders/PBR.vert", "res/shaders/PBR.frag");
		s_Data.PBRShader->Bind();
	}

	void Renderer::Shutdown()
	{
	}

	void Renderer::BeginScene(const OrbitalCamera& camera, const PointLightComponent& light, const TransformComponent& lighttransform)
	{
		s_Data.View = camera.GetViewMatrix();
		s_Data.Projection = camera.GetProjectionMatrix();
		s_Data.Light = light;
		s_Data.LightPos = lighttransform.Translation;

		s_Data.RenderStats.DrawCalls = 0;
	}

	void Renderer::BeginScene(const Camera& camera, const glm::mat4& cameratransform, const PointLightComponent& light, const TransformComponent& lighttransform)
	{
		s_Data.View = glm::inverse(cameratransform);
		s_Data.Projection = camera.GetProjectionMatrix();
		s_Data.Light = light;
		s_Data.LightPos = lighttransform.Translation;

		s_Data.RenderStats.DrawCalls = 0;
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::DrawMesh(const MeshRendererComponent& mesh, const TransformComponent& transform)
	{
		s_Data.PBRShader->Bind();
		s_Data.PBRShader->UploadUniformMat4("u_View", s_Data.View);
		s_Data.PBRShader->UploadUniformMat4("u_Projection", s_Data.Projection);
		s_Data.PBRShader->UploadUniformMat4("u_Model", transform.GetTransform());
		s_Data.PBRShader->UploadUniformFloat3("u_CameraPos", s_Data.CameraPos);

		s_Data.PBRShader->UploadUniformFloat3("u_Albedo", mesh.Material.Albedo);
		s_Data.PBRShader->UploadUniformFloat("u_Metallic", mesh.Material.Metalic);
		s_Data.PBRShader->UploadUniformFloat("u_Roughness", mesh.Material.Roughness);

		s_Data.PBRShader->UploadUniformFloat3("u_LightPosition", s_Data.LightPos);
		s_Data.PBRShader->UploadUniformFloat3("u_LightColor", s_Data.Light.Colour);
		s_Data.PBRShader->UploadUniformFloat("u_LightIntensity", s_Data.Light.Intensity);

		s_Data.Mesh = mesh.Mesh.GetVertexArray();
		
		DrawIndexed();
	}

	void Renderer::DrawIndexed()
	{
		glDrawElements(GL_TRIANGLES, s_Data.Mesh->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		s_Data.RenderStats.DrawCalls++;
	}

	void Renderer::Clear(const glm::vec4& colour)
	{
		glClearColor(colour.r, colour.g, colour.b, colour.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
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