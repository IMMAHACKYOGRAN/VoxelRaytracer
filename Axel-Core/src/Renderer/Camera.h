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
		
		void SetFOV(float fov) { m_VerticalFov = fov; RecalculateProjectionMatrix(); }
		float GetFOV() const { return m_VerticalFov; }
		void SetNearClip(float dist) { m_NearClip = dist; RecalculateProjectionMatrix();}
		float GetNearClip() const { return m_NearClip; }
		void SetFarClip(float dist) { m_FarClip = dist; RecalculateProjectionMatrix(); }
		float GetFarClip() const { return m_FarClip; }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

	private:
		void RecalculateProjectionMatrix();

	private:
		float m_VerticalFov = 45.0f;
		float m_AspectRatio = 16.0f / 9.0f;
		float m_NearClip = 0.1f;
		float m_FarClip = 100.0f;

		glm::mat4 m_ProjectionMatrix{ 1.0f };
	};
}