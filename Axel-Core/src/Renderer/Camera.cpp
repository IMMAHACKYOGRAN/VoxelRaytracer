#include "pch.h"

#include "Camera.h"
#include <gtc/matrix_transform.hpp>
#include <gtx/quaternion.hpp>

namespace Axel
{
	Camera::Camera()
	{
		RecalculateProjectionMatrix();
	}

	Camera::Camera(float vertFov, float aspectRatio, float nearClip, float farClip)
		: m_VerticalFov(vertFov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
	{
		RecalculateProjectionMatrix();
	}

	Camera::~Camera()
	{
	}

	void Camera::ResizeViewport(float width, float height)
	{
		m_AspectRatio = width / height;
		RecalculateProjectionMatrix();
	}

	void Camera::RecalculateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::inverse(glm::perspective(glm::radians(m_VerticalFov), m_AspectRatio, m_NearClip, m_FarClip));
	}
}