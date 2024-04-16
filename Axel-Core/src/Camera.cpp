#include "pch.h"

#include "Camera.h"

#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>

namespace Axel
{
	Camera::Camera(float verticalFOV, float nearClip, float farClip)
		: m_VerticalFOV(verticalFOV), m_NearClip(nearClip), m_FarClip(farClip)
	{
		m_ForwardDirection = glm::vec3(0, 0, -1);
	}

	Camera::~Camera()
	{
	}

	void Camera::OnUpdate(float dt)
	{
		glm::vec2 mousePos = Input::GetMousePosition();
		glm::vec2 delta = (mousePos - m_LastMousePosition);
		m_LastMousePosition = mousePos;

		bool moved = false;

		constexpr glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
		glm::vec3 rightDirection = glm::cross(m_ForwardDirection, upDirection);

		float speed = 1.0f;
		float sensitivity = 0.002f;

		if (Input::IsKeyDown(KeyCode::R))
			m_ShouldUpdate = !m_ShouldUpdate;

		if (!m_ShouldUpdate)
			return;

		if (Input::IsMouseButtonDown(MouseButton::Left))
		{
			Input::SetCursorMode(CursorMode::Locked);

			if (delta.x != 0.0f || delta.y != 0.0f)
			{
				float pitchDelta = delta.y * sensitivity;
				float yawDelta = delta.x * sensitivity;

				glm::quat rot = glm::normalize(glm::cross(glm::angleAxis(-pitchDelta, rightDirection), glm::angleAxis(-yawDelta, upDirection)));
				m_ForwardDirection = glm::rotate(rot, m_ForwardDirection);

				moved = true;
			}
		}
		else
		{
			Input::SetCursorMode(CursorMode::Normal);
		}

		if (Input::IsKeyDown(KeyCode::W))
		{
			m_Position += m_ForwardDirection * speed * dt;
			moved = true;
		}
		if (Input::IsKeyDown(KeyCode::S))
		{
			m_Position -= m_ForwardDirection * speed * dt;
			moved = true;
		}
		if (Input::IsKeyDown(KeyCode::A))
		{
			m_Position -= rightDirection * speed * dt;
			moved = true;
		}
		if (Input::IsKeyDown(KeyCode::D))
		{
			m_Position += rightDirection * speed * dt;
			moved = true;
		}
		if (Input::IsKeyDown(KeyCode::E))
		{
			m_Position += upDirection * speed * dt;
			moved = true;
		}
		if (Input::IsKeyDown(KeyCode::Q))
		{
			m_Position -= upDirection * speed * dt;
			moved = true;
		}

		if (moved)
		{
			RecalculateProjectionMatrix();
			RecalculateViewMatrix();
		}
	}

	void Camera::OnResize(uint32_t width, uint32_t height)
	{
		if (width == m_ViewportWidth && height == m_ViewportHeight)
			return;

		m_ViewportWidth = width;
		m_ViewportHeight = height;

		RecalculateProjectionMatrix();
		RecalculateViewMatrix();
	}

	void Camera::RecalculateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::perspectiveFov(glm::radians(m_VerticalFOV), (float)m_ViewportWidth, (float)m_ViewportHeight, m_NearClip, m_FarClip);
		m_InverseProjectionMatrix = glm::inverse(m_ProjectionMatrix);
	}

	void Camera::RecalculateViewMatrix()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_ForwardDirection, glm::vec3(0, 1, 0));
		m_InverseViewMatrix = glm::inverse(m_ViewMatrix);
	}
}