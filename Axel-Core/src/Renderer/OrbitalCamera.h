#pragma once
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include <glm.hpp>
#include <gtx/quaternion.hpp>

namespace Axel 
{
	class OrbitalCamera
	{
	public:
		OrbitalCamera();
		OrbitalCamera(float verticalFov, float aspectRatio, float nearClip, float farClip);
		~OrbitalCamera();

		void OnUpdate(float dt);
		void OnEvent(Event& e);
		bool OnMouseScroll(MouseScrolledEvent& e);

		void ResizeViewport(float width, float height);

		void SetFocalPoint(const glm::vec3& point);

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewProjection() const { return m_ProjectionMatrix * m_ViewMatrix; }

		const glm::vec3 GetUpDirection() const;
		const glm::vec3 GetRightDirection() const;
		const glm::vec3 GetForwardDirection() const;

		const glm::vec3 GetPosition() const { return m_Position; }
		const glm::quat GetOrientation() const { return m_Orientation; }

	private:
		void RecalculateProjectionMatrix();
		void RecalculateViewMatrix();
		
	private:
		float m_VerticalFov = 45.0f;
		float m_AspectRatio = 16.0f/9.0f;
		float m_NearClip = 0.1f;
		float m_FarClip = 100.0f;

		glm::mat4 m_ViewMatrix{ 1.0f };
		glm::mat4 m_ProjectionMatrix{ 1.0f };
		glm::vec3 m_Position{ 0.0f };
		glm::vec3 m_FocalPoint{ 0.0f };
		glm::quat m_Orientation;

		glm::vec2 m_MousePos = { 0.0f, 0.0f };

		float m_Distance = 10.0f;
		float m_Pitch = 0.75f;
		float m_Yaw = -0.75f;

		float m_ViewportWidth = 1280;
		float m_ViewportHeight = 720;
	};

}