#include "pch.h"
#include "Renderer/OrbitalCamera.h"
#include "Core/Input.h"

namespace Axel
{
	OrbitalCamera::OrbitalCamera(float verticalFov, float aspectRatio, float nearClip, float farClip)
		: m_VerticalFov(verticalFov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
	{
		RecalculateViewMatrix();
		RecalculateProjectionMatrix();
	}

	OrbitalCamera::~OrbitalCamera()
	{
	}

	void OrbitalCamera::RecalculateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(m_VerticalFov), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void OrbitalCamera::RecalculateViewMatrix()
	{
		m_Position = m_FocalPoint - GetForwardDirection() * m_Distance;
		m_Orientation = glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));

		m_ViewMatrix = glm::inverse(glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(m_Orientation));
	}

	void OrbitalCamera::OnUpdate(float dt)
	{
		const glm::vec2 mouse = Input::GetMousePosition();
		glm::vec2 delta = (mouse - m_MousePos) * 0.003f;
		m_MousePos = mouse;

		if (Input::IsMouseButtonDown(MouseButton::Middle))
		{
			m_FocalPoint += -GetRightDirection() * delta.x * 0.1f * m_Distance;
			m_FocalPoint += GetUpDirection() * delta.y * 0.1f * m_Distance;

			RecalculateViewMatrix();
		}
		else if (Input::IsMouseButtonDown(MouseButton::Left))
		{
			float up = GetUpDirection().y < 0 ? -1.0f : 1.0f;
			m_Yaw += up * delta.x * 0.8f;
			m_Pitch += delta.y * 0.8f;

			RecalculateViewMatrix();
		}
	}

	bool OrbitalCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		m_Distance -= e.GetYOffset();
		if (m_Distance < 1.0f)
		{
			//m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}

		RecalculateViewMatrix();

		return false;
	}

	void OrbitalCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&OrbitalCamera::OnMouseScroll, this, std::placeholders::_1));
	}

	void OrbitalCamera::ResizeViewport(float width, float height)
	{
		m_AspectRatio = width / height;
		RecalculateProjectionMatrix();
	}

	const glm::vec3 OrbitalCamera::GetUpDirection() const
	{
		return glm::rotate(m_Orientation, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	const glm::vec3 OrbitalCamera::GetRightDirection() const
	{
		return glm::rotate(m_Orientation, glm::vec3(1.0f, 0.0f, 0.0f));
	}

	const glm::vec3 OrbitalCamera::GetForwardDirection() const
	{
		return glm::rotate(m_Orientation, glm::vec3(0.0f, 0.0f, -1.0f));
	}
}