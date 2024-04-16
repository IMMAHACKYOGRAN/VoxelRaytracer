#pragma once
#include <glm.hpp>

namespace Axel
{
	class Camera
	{
	public:
		Camera(float verticalFOV, float nearClip, float farClip);
		~Camera();

		void OnUpdate(float dt);
		void OnResize(uint32_t width, uint32_t height);

		void SetPosition(glm::vec3 pos) { m_Position = pos; }
		glm::vec3 GetPosition() { return m_Position; }
		glm::vec3 GetDirection() { return m_ForwardDirection; }

		glm::mat4 GetProjectionMatrix() { return m_ProjectionMatrix; }
		glm::mat4 GetInverseProjectionMatrix() { return m_InverseProjectionMatrix; }
		glm::mat4 GetViewMatrix() { return m_ViewMatrix; }
		glm::mat4 GetInverseViewMatrix() { return m_InverseViewMatrix; }

	private:
		void RecalculateProjectionMatrix();
		void RecalculateViewMatrix();

	private:
		glm::vec3 m_Position{ 0.0f };
		glm::vec3 m_ForwardDirection{ 0.0f };

		glm::mat4 m_ProjectionMatrix{ 1.0f };
		glm::mat4 m_InverseProjectionMatrix{ 1.0f };
		glm::mat4 m_ViewMatrix{ 1.0f };
		glm::mat4 m_InverseViewMatrix{ 1.0f };

		float m_VerticalFOV = 45.0f;
		float m_NearClip = 0.1f;
		float m_FarClip = 100.0f;

		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		glm::vec2 m_LastMousePosition{ 0.0f };

		bool m_ShouldUpdate = true;
	};
}