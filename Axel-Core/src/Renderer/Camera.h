#pragma once
#include <glm.hpp>

namespace Axel
{
	class Camera
	{
	public:
		Camera();
		Camera(float vertFov, float aspectRatio, float nearClip, float farClip);
		~Camera();

		void ResizeViewport(float width, float height);

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

	private:
		void RecalculateProjectionMatrix();

	private:
		float m_VerticalFov = 45.0f;
		float m_AspectRatio = 16.0f / 9.0f;
		float m_NearClip = 0.1f;
		float m_FarClip = 100.0f;

		glm::mat4 m_ProjectionMatrix{ 1.0f };
		glm::vec3 m_Position{ 0.0f };
	};
}