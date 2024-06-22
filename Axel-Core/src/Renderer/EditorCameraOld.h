#pragma once

#include "Events/Event.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"

#include <glm.hpp>

namespace Axel
{
	class EditorCamera
	{
	public:
		EditorCamera(float verticalFOV, float aspectRatio, float nearClip, float farClip);
		~EditorCamera();

		void OnUpdate(float dt);
		void OnEvent(Event& e);

		void ResizeViewport(float width, float height);

		void SetPosition(glm::vec3 pos) { m_Position = pos; }
		glm::vec3 GetPosition() const { return m_Position; }
		glm::vec3 GetDirection() const { return m_ForwardDirection; }

		glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
		glm::mat4 GetInverseProjectionMatrix() const { return m_InverseProjectionMatrix; }
		glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetInverseViewMatrix() const { return m_InverseViewMatrix; }

	private:
		bool OnResize(WindowResizeEvent& e);

		void RecalculateProjectionMatrix();
		void RecalculateViewMatrix();

	private:
		glm::vec3 m_Position{ 0.0f };
		glm::vec3 m_ForwardDirection{ 0, 0, -1 };

		glm::mat4 m_ProjectionMatrix{ 1.0f };
		glm::mat4 m_InverseProjectionMatrix{ 1.0f };
		glm::mat4 m_ViewMatrix{ 1.0f };
		glm::mat4 m_InverseViewMatrix{ 1.0f };

		float m_VerticalFOV = 45.0f;
		float m_NearClip = 0.1f;
		float m_FarClip = 100.0f;
		float m_AspectRatio = 16.0/9.0f;

		glm::vec2 m_LastMousePosition{ 0.0f };
	};
}