#pragma once

#include "Camera.h"
#include "VoxelModel.h"

#include "Renderer/Shader.h"
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

	void OnResize();

	static Application& Get();
	GLFWwindow* GetWindow() const { return m_Window; }

	uint32_t GetWidth() const { return m_WindowData.Width; }
	uint32_t GetHeight() const { return m_WindowData.Height; }

private:
	void Init();
	void Shutdown();

private:
	ApplicationSpecification m_Specification;
	GLFWwindow* m_Window;

	ImGuiLayer* m_ImGuiLayer;

	bool m_Running = false;
	bool m_Minimised = false;

	std::shared_ptr<VertexArray> m_VertexArray;
	std::shared_ptr<VertexBuffer> m_VertexBuffer;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;

	Camera m_Camera;
	std::shared_ptr<Shader> m_Shader;

	std::shared_ptr<VoxelModel> m_Crate;

	struct WindowData
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		bool Resize = false;
	};

	WindowData m_WindowData;

	float m_LastTime = 0;

	//tmp
	glm::mat4 m_ModelMat = glm::mat4(1.0f);
};