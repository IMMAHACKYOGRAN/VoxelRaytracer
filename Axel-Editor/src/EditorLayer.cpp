#include "EditorLayer.h"
#include "../res/assets/scripts/test.h"
#include <gtc/type_ptr.hpp>

EditorLayer::EditorLayer()
	: m_Camera(30.0f, 1.7f, 0.01f, 100.0f)
{
    ImGuiIO& io = ImGui::GetIO();
    m_Font = io.Fonts->AddFontFromFileTTF("res/assets/fonts/Roboto-Regular.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesDefault());

	m_Scene = std::shared_ptr<Axel::Scene>(new Axel::Scene);
	m_NewEntity = m_Scene->CreateEntity();
    auto& sc = m_NewEntity.AddComponent<Axel::ScriptComponent>();
    sc.Script = new test;
	m_NewEntity.AddComponent<Axel::TransformComponent>();
}

void EditorLayer::OnAttach()
{
	Axel::FrameBufferSpecification fbSpec;
	fbSpec.Attachments = { Axel::AttachmentFormat::RGBA, Axel::AttachmentFormat::RED_INTEGER, Axel::AttachmentFormat::DEPTH };
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_FrameBuffer = Axel::FrameBuffer::Create(fbSpec);
}

void EditorLayer::OnEvent(Axel::Event& e)
{
    if(m_ViewportFocused)
	    m_Camera.OnEvent(e);
}

void EditorLayer::OnUpdate(float dt)
{
	m_Dt = dt;
    
    //if(m_ViewportFocused)
    m_Camera.OnUpdate(dt);

    m_Scene->OnEditorPlayUpdate(dt); // TEMP

	m_FrameBuffer->Bind();
	Axel::Renderer::Clear({ 0.1f, 0.1f, 0.1f, 1.0f });

	Axel::Renderer::BeginScene(m_Camera);

	for (const auto entity : m_Scene->GetEntitiesWith<Axel::TransformComponent>())
		Axel::Renderer::DrawCube(m_Scene->GetComponent<Axel::TransformComponent>(entity));

	Axel::Renderer::EndScene();
	m_FrameBuffer->Unbind();
}

void EditorLayer::OnImGuiRender()
{
    bool dockspaceOpen = true;
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGui::PushFont(m_Font);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiID dockspace_id = ImGui::GetID("Dockspace");
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 100.0f;
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    style.WindowMinSize.x = minWinSizeX;

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Project")) { /*Create new project*/ }
            if (ImGui::MenuItem("Open Project", "Ctrl+O")) { /*Open existing project*/ }
            if (ImGui::MenuItem("New Scene", "Ctrl+N")) { /*Open existing project*/ }
            if (ImGui::MenuItem("Save Scene", "Ctrl+S")) { /*Open existing project*/ }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) { Axel::Application::Get().Close(); }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo")) {}
            if (ImGui::MenuItem("Redo")) {}
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Viewport");
    ImVec2 min= ImGui::GetWindowContentRegionMin();
    ImVec2 max = ImGui::GetWindowContentRegionMax();
    ImVec2 offset = ImGui::GetWindowPos();
    m_ViewportBounds[0] = { min.x + offset.x, min.y + offset.y };
    m_ViewportBounds[1] = { max.x + offset.x, max.y + offset.y };

    m_ViewportFocused = ImGui::IsWindowFocused();

    Axel::Application::Get().GetUILayer()->BlockImGuiLayerEvents(true);

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

    if (m_ViewportSize != glm::vec2{ viewportPanelSize.x, viewportPanelSize.y })
    {
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        m_FrameBuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
        //Axel::Renderer::SetViewport(0, 0, (uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
        m_Camera.ResizeViewport(m_ViewportSize.x, m_ViewportSize.y);
    }

    uint32_t textureID = m_FrameBuffer->GetAttachmentRendererID(0);
    ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    ImGui::End(); // Viewport
    ImGui::PopStyleVar();

	ImGui::Begin("Debug");
	ImGui::Text("Rendering Stats");
	ImGui::Separator();
	ImGui::Text("Frame Time: %f", m_Dt);
	ImGui::Text("FPS: %d", (int)(1.0f / m_Dt));
	ImGui::Text("Draw Calls: %d", Axel::Renderer::GetStats().DrawCalls);
    ImGui::Separator();
    ImGui::Text("Entity");
    ImGui::Separator();
    ImGui::DragFloat3("Position", glm::value_ptr(m_NewEntity.GetComponent<Axel::TransformComponent>().Translation), 0.01f);
	ImGui::End(); // Debug

    ImGui::End(); // Docspace
    ImGui::PopFont();
}