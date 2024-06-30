#pragma once
#include <Axel.h>
#include "UIPanels/PropertiesScenePanel.h"
#include <ImGuizmo/ImGuizmo.h>

class EditorLayer : public Axel::Layer
{
public:
	EditorLayer();

	virtual void OnAttach() override;
	
	virtual void OnUpdate(float dt) override;
	virtual void OnEvent(Axel::Event& e) override;
	
	virtual void OnImGuiRender() override;

	void DrawUIButtons();

private:
	std::string m_WorkingDirectory = std::string();

	std::shared_ptr<Axel::Texture2D> m_PlayButtonImage;
	std::shared_ptr<Axel::Texture2D> m_StopButtonImage;

	std::shared_ptr<Axel::Texture2D> m_TranslateButtonImage;
	std::shared_ptr<Axel::Texture2D> m_RotateButtonImage;
	std::shared_ptr<Axel::Texture2D> m_ScaleButtonImage;

	enum class EditorState
	{
		Editing,
		Running,
	} m_EditorState = EditorState::Editing;

	Axel::OrbitalCamera m_EditorCamera;

	std::shared_ptr<Axel::Scene> m_CurrentScene;
	std::shared_ptr<Axel::FrameBuffer> m_FrameBuffer;
	Axel::EntityId m_SelectedEntity;

	PropertiesScenePanel m_PropertiesScenePanel;
	ImGuizmo::OPERATION m_CurrentGizmo = ImGuizmo::OPERATION::TRANSLATE;

	ImFont* m_Font;

	glm::vec2 m_ViewportSize;

	bool m_ViewportFocused = false;
	bool m_ViewportHovered = false;

	bool m_UpdateCam = true;

	float m_Dt = 0; //TODO: remove
};

static void DecomposeMatrix(const glm::mat4& m, glm::vec3& pos, glm::quat& rot, glm::vec3& scale)
{
	pos = m[3];
	for (int i = 0; i < 3; i++)
		scale[i] = glm::length(glm::vec3(m[i]));
	const glm::mat3 rotMtx(
		glm::vec3(m[0]) / scale[0],
		glm::vec3(m[1]) / scale[1],
		glm::vec3(m[2]) / scale[2]);
	rot = glm::quat_cast(rotMtx);
}