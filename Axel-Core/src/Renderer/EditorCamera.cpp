#include "pch.h"

#include "EditorCamera.h"

#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>

#include "Core/Input.h"

namespace Axel
{
	EditorCamera::EditorCamera(float verticalFOV, float nearClip, float farClip, float vpWidth, float vpHeight)
		//TODO: Remove the need for viewport w&h to be set manualy.
		: m_VerticalFOV(verticalFOV), m_NearClip(nearClip), m_FarClip(farClip), m_ViewportWidth(vpWidth), m_ViewportHeight(vpHeight)
	{
		m_ForwardDirection = glm::vec3(0, 0, -1);
		RecalculateProjectionMatrix();
		RecalculateViewMatrix();
	}

	EditorCamera::~EditorCamera()
	{
	}

	void EditorCamera::OnUpdate(float dt)
	{
		glm::vec2 mousePos = Input::GetMousePosition();
		glm::vec2 delta = (mousePos - m_LastMousePosition);
		m_LastMousePosition = mousePos;

		bool moved = false;

		constexpr glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
		glm::vec3 rightDirection = glm::cross(m_ForwardDirection, upDirection);

		float speed = 1.0f;
		float sensitivity = 0.002f;

		if (Input::IsKeyDown(Key::R))
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

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&EditorCamera::OnResize, this, std::placeholders::_1));
	}

	bool EditorCamera::OnResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == m_ViewportWidth && e.GetHeight() == m_ViewportHeight)
			return false;

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
			return false;

		m_ViewportWidth = e.GetWidth();
		m_ViewportHeight = e.GetHeight();

		RecalculateProjectionMatrix();
		RecalculateViewMatrix();

		return false;
	}

	void EditorCamera::RecalculateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::perspectiveFov(glm::radians(m_VerticalFOV), (float)m_ViewportWidth, (float)m_ViewportHeight, m_NearClip, m_FarClip);
		m_InverseProjectionMatrix = glm::inverse(m_ProjectionMatrix);
	}

	void EditorCamera::RecalculateViewMatrix()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_ForwardDirection, glm::vec3(0, 1, 0));
		m_InverseViewMatrix = glm::inverse(m_ViewMatrix);
	}
}