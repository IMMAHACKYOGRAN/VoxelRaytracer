#include <Axel.h>

class ExampleLayer : public Axel::Layer
{
public:
	ExampleLayer()
		: m_Camera(45.0f, 0.01f, 100.0f)
	{
		
	}

	~ExampleLayer()
	{

	}

	void OnAttach()
	{

	}

	void OnUpdate(float dt)
	{
		Axel::Renderer::Clear({ 0.1f, 0.1f, 0.1f, 1.0f });

		Axel::Renderer::BeginScene(m_Camera);

		Axel::Renderer::DrawCube(volume);

		Axel::Renderer::EndScene();
	}

	void OnImGuiRender()
	{
		ImGui::ShowDemoWindow();
	}

private:
	Axel::EditorCamera m_Camera;
	Axel::VoxelVolume volume;
};

class AxelEditor : public Axel::Application
{
public:
	AxelEditor()
	{
		PushLayer(new ExampleLayer());
	}

	~AxelEditor()
	{

	}
};

Axel::Application* Axel::CreateApplication()
{
	return new AxelEditor();
}