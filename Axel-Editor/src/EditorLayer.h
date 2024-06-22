#pragma once
#include <Axel.h>

class EditorLayer : public Axel::Layer
{
public:
	EditorLayer();

	virtual void OnAttach() override;
	
	virtual void OnUpdate(float dt) override;
	virtual void OnEvent(Axel::Event& e) override;
	
	virtual void OnImGuiRender() override;

private:
	Axel::OrbitalCamera m_Camera;

	std::shared_ptr<Axel::Scene> m_Scene;
	std::shared_ptr<Axel::FrameBuffer> m_FrameBuffer;
	Axel::Entity m_NewEntity;

	ImFont* m_Font;

	glm::vec2 m_ViewportBounds[2];
	glm::vec2 m_ViewportSize;

	bool m_ViewportFocused = false;

	float m_Dt = 0; //TODO: remove
};