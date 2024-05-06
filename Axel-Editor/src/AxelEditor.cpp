#include <Axel.h>

class ExampleLayer : public Axel::Layer
{
public:
	ExampleLayer()
		: m_Camera(45.0f, 0.01f, 100.0f, Axel::Application::Get().GetWindow().GetWidth(), Axel::Application::Get().GetWindow().GetHeight())
	{
		transform.Translate = { 0.0f, -1.0f, -1.0f };
	}

	~ExampleLayer()
	{

	}

	void OnAttach()
	{

	}

	void OnEvent(Axel::Event& e) override
	{
		m_Camera.OnEvent(e);
	}

	void OnUpdate(float dt)
	{
		m_Dt = dt;
		m_Camera.OnUpdate(dt);

		Axel::Renderer::Clear({ 0.1f, 0.1f, 0.1f, 1.0f });

		Axel::Renderer::BeginScene(m_Camera);

		Axel::Renderer::DrawCube(transform);
		Axel::Renderer::DrawCube(transform2);

		Axel::Renderer::EndScene();
	}

	void OnImGuiRender()
	{
		ImGui::Begin("Debug");
		ImGui::Text("Rendering Stats");
		ImGui::Separator();
		ImGui::Text("Frame Time: %f", m_Dt);
		ImGui::Text("FPS: %d", (int)(1.0f / m_Dt));
		ImGui::Text("Draw Calls: %d", Axel::Renderer::GetStats().DrawCalls);
		ImGui::End();
	}

private:
	Axel::EditorCamera m_Camera;
	Axel::Transform transform;
	Axel::Transform transform2;

	float m_Dt = 0; //TODO: remove
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