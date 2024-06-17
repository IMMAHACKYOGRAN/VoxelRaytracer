#include <Axel.h>

class ExampleLayer : public Axel::Layer
{
public:
	ExampleLayer()
		: m_Camera(45.0f, 0.01f, 100.0f, Axel::Application::Get().GetWindow().GetWidth(), Axel::Application::Get().GetWindow().GetHeight())
	{
		m_Scene = std::unique_ptr<Axel::Scene>(new Axel::Scene);
		m_NewEntity = m_Scene->CreateEntity();
		Axel::TransformComponent& trans = m_NewEntity.AddComponent<Axel::TransformComponent>();
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

		//for (auto entity : m_Scene->GetEntitiesWith<Axel::TransformComponent>())
		Axel::Renderer::DrawCube(m_NewEntity.GetComponent<Axel::TransformComponent>());

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

	std::unique_ptr<Axel::Scene> m_Scene;
	Axel::Entity m_NewEntity;
	Axel::TransformComponent trans;

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