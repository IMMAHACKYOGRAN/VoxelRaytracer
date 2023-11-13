#pragma once

#include "Camera.h"
#include "Shader.h"
#include "Renderer/VertexArray.h"

#include "ImGui/ImGuiLayer.h"

struct ApplicationSpecification
{
	std::string Name = "App";
	uint32_t Width = 1600;
	uint32_t Height = 900;
};

class Application
{
public:
	Application(const ApplicationSpecification& applicationSpecification = ApplicationSpecification());
	~Application();

	void Run();
	void Close();

	static Application& Get();
	GLFWwindow* GetWindow() const { return m_Window; }

	uint32_t GetWidth() const { return m_Width; }
	uint32_t GetHeight() const { return m_Height; }
		
private:
	void Init();
	void Shutdown();

private:
	ApplicationSpecification m_Specification;
	GLFWwindow* m_Window;

	ImGuiLayer* m_ImGuiLayer;

	bool m_Running = false;

	std::shared_ptr<VertexArray> m_VertexArray;
	std::shared_ptr<VertexBuffer> m_VertexBuffer;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;

	Camera m_Camera;
	std::shared_ptr<Shader> m_Shader;

	uint32_t m_Width;
	uint32_t m_Height;

	float m_LastTime = 0;

	//tmp
	glm::mat4 m_ModelMat = glm::mat4(1.0f);
};