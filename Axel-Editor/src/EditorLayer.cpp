#include "EditorLayer.h"
#include "../res/assets/scripts/test.h"
#include <gtc/type_ptr.hpp>

EditorLayer::EditorLayer()
	: m_EditorCamera(30.0f, 1.7f, 0.01f, 100.0f)
{
    ImGuiIO& io = ImGui::GetIO();
    m_Font = io.Fonts->AddFontFromFileTTF("res/assets/fonts/Roboto-Regular.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesDefault());
}

void EditorLayer::OnAttach()
{
	Axel::FrameBufferSpecification fbSpec;
	fbSpec.Attachments = { Axel::AttachmentFormat::RGBA, Axel::AttachmentFormat::RED_INTEGER, Axel::AttachmentFormat::DEPTH };
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_FrameBuffer = Axel::FrameBuffer::Create(fbSpec);

    int w, h;
    uint8_t* pixels = Texture2D::LoadTexture("res/assets/imgs/Icon.png", &w, &h);
    Application::Get().SetIcon(pixels, w, h);

    m_PlayButtonImage = std::make_shared<Axel::Texture2D>("res/assets/imgs/PlayButton.png");
    m_StopButtonImage = std::make_shared<Axel::Texture2D>("res/assets/imgs/StopButton.png");

    m_CurrentScene = std::shared_ptr<Axel::Scene>(new Axel::Scene);
    m_PropertiesScenePanel.SetScene(m_CurrentScene);
    m_PropertiesScenePanel.SetEditorCamera(m_EditorCamera);
}

void EditorLayer::OnEvent(Axel::Event& e)
{
    if(m_ViewportHovered && m_EditorState == EditorState::Editing)
        m_EditorCamera.OnEvent(e);
}

void EditorLayer::OnUpdate(float dt)
{
	m_Dt = dt;

    m_FrameBuffer->Bind();
    Axel::Renderer::Clear({ 0.1f, 0.1f, 0.1f, 1.0f });
    
    switch (m_EditorState)
    {
        case EditorState::None:
        {
            AX_ERROR("How does this happen?");
            break;
        }
        case EditorState::Editing:
        {
            if (m_ViewportFocused)
                m_EditorCamera.OnUpdate(dt);

            m_CurrentScene->OnEditorUpdate(dt, m_EditorCamera);
            break;

        }
        case EditorState::Running:
        {
            m_CurrentScene->OnEditorPlayUpdate(dt);
            break;
        }
    }

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
            if (ImGui::MenuItem("Save Scene", "Ctrl+S")) 
            {
                Axel::Serializer s(m_CurrentScene);
                s.Serialize("res/assets/scenes/text.axl");
            }
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

    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

    if (m_ViewportSize != glm::vec2{ viewportPanelSize.x, viewportPanelSize.y })
    {
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        m_FrameBuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
        m_EditorCamera.ResizeViewport(m_ViewportSize.x, m_ViewportSize.y);
    }

    uint32_t textureID = m_FrameBuffer->GetAttachmentRendererID(0);
    ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    DrawUIButtons();

    ImGui::End(); // Viewport
    ImGui::PopStyleVar();

	ImGui::Begin("Debug");
	ImGui::Text("Rendering Stats");
	ImGui::Separator();
	ImGui::Text("Frame Time: %f", m_Dt);
	ImGui::Text("FPS: %d", (int)(1.0f / m_Dt));
	ImGui::Text("Draw Calls: %d", Axel::Renderer::GetStats().DrawCalls);
    ImGui::Separator();
    if (ImGui::Button("Reset"))
        m_EditorCamera.SetFocalPoint({0.0f, 0.0f, 0.0f});
	ImGui::End(); // Debug

    m_PropertiesScenePanel.Draw();

    ImGui::End(); // Docspace
    ImGui::PopFont();
}

void EditorLayer::DrawUIButtons()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    auto& colors = ImGui::GetStyle().Colors;
    const ImVec4& buttonHovered = colors[ImGuiCol_ButtonHovered];
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
    const ImVec4& buttonActive = colors[ImGuiCol_ButtonActive];
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

    ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    ImVec4 tintColor = ImVec4(1, 1, 1, 1);

    float size = ImGui::GetWindowHeight() - 4.0f;
    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

    std::shared_ptr<Axel::Texture2D> icon = m_EditorState == EditorState::Editing ? m_PlayButtonImage : m_StopButtonImage;
    if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor))
    {
        if (m_EditorState == EditorState::Editing)
        {
            m_CurrentScene->OnEditorPlayStart();
            m_EditorState = EditorState::Running;
        }
        else
            m_EditorState = EditorState::Editing;
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);
    ImGui::End();
}