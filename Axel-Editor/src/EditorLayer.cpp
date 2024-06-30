#include "EditorLayer.h"
#include "../res/assets/scripts/test.h"
#include <gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

// File dialogs
#include <commdlg.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

// From https://stackoverflow.com/questions/4167286/win32-function-to-openfiledialog
static std::string OpenAxelFile()
{
    OPENFILENAMEA ofn;
    CHAR szFile[260] = { 0 };
    ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.hwndOwner = glfwGetWin32Window(Axel::Application::Get().GetWindow().GetNativeWindow());
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Axel Scene (*.axl)\0*.axl\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    if (GetOpenFileNameA(&ofn) == TRUE)
    {
        return ofn.lpstrFile;
    }
    return std::string();
}

static std::string SaveAxelFile()
{
    OPENFILENAMEA ofn;
    CHAR szFile[260] = { 0 };
    CHAR currentDir[256] = { 0 };
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    if (GetCurrentDirectoryA(256, currentDir))
        ofn.lpstrInitialDir = currentDir;
    ofn.lpstrFilter = "Axel Scene (*.axl)\0*.axl\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

    // Sets the default extension by extracting it from the filter
    ofn.lpstrDefExt = strchr("Axel Scene (*.axl)\0*.axl\0", '\0') + 1;

    if (GetSaveFileNameA(&ofn) == TRUE)
        return ofn.lpstrFile;

    return std::string();
}

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
    m_TranslateButtonImage = std::make_shared<Axel::Texture2D>("res/assets/imgs/Translate.png");
    m_RotateButtonImage = std::make_shared<Axel::Texture2D>("res/assets/imgs/Rotate.png");
    m_ScaleButtonImage = std::make_shared<Axel::Texture2D>("res/assets/imgs/Scale.png");

    m_CurrentScene = std::shared_ptr<Axel::Scene>(new Axel::Scene);
    m_PropertiesScenePanel.SetScene(m_CurrentScene);
    m_PropertiesScenePanel.SetEditorCamera(m_EditorCamera);

    Entity e = m_CurrentScene->CreateEntity();
    e.AddComponent<CameraComponent>();
    m_CurrentScene->SetMainCameraEntity((EntityId)e);
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
        case EditorState::Editing:
        {
            if (m_ViewportFocused && m_UpdateCam)
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
            if (ImGui::MenuItem("Open Scene")) 
            {
                std::string path = OpenAxelFile();
                if (!path.empty())
                {
                    m_WorkingDirectory = path;
                    std::shared_ptr<Axel::Scene> newscene = std::make_shared<Axel::Scene>();
                    Axel::Serializer s(newscene);
                    s.Deserialize(path, m_EditorCamera);
                    m_CurrentScene = newscene;
                    m_PropertiesScenePanel.SetScene(m_CurrentScene);
                    m_EditorState = EditorState::Editing;
                    m_EditorCamera.ResizeViewport(m_ViewportSize.x, m_ViewportSize.y);
                }
            }

            if (ImGui::MenuItem("New Scene")) 
            {
                m_WorkingDirectory = std::string();
                std::shared_ptr<Axel::Scene> newscene = std::make_shared<Axel::Scene>();
                m_CurrentScene = newscene;
                m_PropertiesScenePanel.SetScene(m_CurrentScene);
                m_EditorState = EditorState::Editing;
                m_EditorCamera.ResizeViewport(m_ViewportSize.x, m_ViewportSize.y);
            }

            if (ImGui::MenuItem("Save Scene")) 
            {
                if (m_WorkingDirectory.empty())
                {
                    std::string path = SaveAxelFile();
                    if (!path.empty())
                    {
                        m_WorkingDirectory = path;
                        Axel::Serializer s(m_CurrentScene);
                        s.Serialize(m_WorkingDirectory, m_EditorCamera);
                    }
                }
                else
                {
                    Axel::Serializer s(m_CurrentScene);
                    s.Serialize(m_WorkingDirectory, m_EditorCamera);
                }
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
    ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_MenuBar);
    ImVec2 min = ImGui::GetWindowContentRegionMin();
    ImVec2 max = ImGui::GetWindowContentRegionMax();

    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

    if (m_ViewportSize != glm::vec2{ viewportPanelSize.x, viewportPanelSize.y })
    {
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        m_FrameBuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
        Axel::Renderer::SetViewport(0, 0, viewportPanelSize.x, viewportPanelSize.y);
        m_EditorCamera.ResizeViewport(m_ViewportSize.x, m_ViewportSize.y);
        if (m_CurrentScene->IsMainCameraEntity())
            m_CurrentScene->GetComponent<CameraComponent>(m_CurrentScene->GetMainCameraEntity()).Cam.ResizeViewport(m_ViewportSize.x, m_ViewportSize.y);
    }

    DrawUIButtons();
    ImGui::SetCursorPos(ImGui::GetWindowContentRegionMin());
    uint32_t textureID = m_FrameBuffer->GetAttachmentRendererID(0);
    ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    if (m_PropertiesScenePanel.IsEntitySelected() && m_EditorState == EditorState::Editing)
    {
        m_SelectedEntity = m_PropertiesScenePanel.GetSelectedEntity();
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

        const glm::mat4& cameraProjection = m_EditorCamera.GetProjectionMatrix();
        glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

        auto& tc = m_CurrentScene->GetComponent<TransformComponent>(m_SelectedEntity);
        glm::mat4 transform = tc.GetTransform();

        ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
            m_CurrentGizmo, ImGuizmo::LOCAL, glm::value_ptr(transform),
            nullptr, nullptr);

        m_UpdateCam = true;
        if (ImGuizmo::IsUsing())
        {
            m_UpdateCam = false;
            glm::quat qrotation;
            glm::vec3 translation{}, scale{};

            DecomposeMatrix(transform, translation, qrotation, scale);

            glm::vec3 rotation = glm::eulerAngles(qrotation);

            glm::vec3 deltaRotation = rotation - tc.Rotation;
            tc.Translation = translation;
            tc.Rotation += deltaRotation;
            tc.Scale = scale;
        }
    }

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
    float size = 16.0f;

    ImGui::BeginMenuBar();

    if (ImGui::ImageButton((ImTextureID)(uint64_t)m_TranslateButtonImage->GetRendererID(), ImVec2(size, size))) { m_CurrentGizmo = ImGuizmo::OPERATION::TRANSLATE; }
    if (ImGui::ImageButton((ImTextureID)(uint64_t)m_RotateButtonImage->GetRendererID(), ImVec2(size, size))) { m_CurrentGizmo = ImGuizmo::OPERATION::ROTATE; }
    if (ImGui::ImageButton((ImTextureID)(uint64_t)m_ScaleButtonImage->GetRendererID(), ImVec2(size, size))) { m_CurrentGizmo = ImGuizmo::OPERATION::SCALE; }

    ImGui::SetCursorPosX((ImGui::GetWindowWidth() / 2.0f) - (size + 2.0f));
    std::shared_ptr<Axel::Texture2D> icon = m_EditorState == EditorState::Editing ? m_PlayButtonImage : m_StopButtonImage;
    if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size)))
    {
        if (m_EditorState == EditorState::Editing)
        {
            m_CurrentScene->OnEditorPlayStart();
            m_EditorState = EditorState::Running;
        }
        else
            m_EditorState = EditorState::Editing;
    }

    ImGui::EndMenuBar();
}