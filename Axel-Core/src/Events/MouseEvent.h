#pragma once

#include "Event.h"

namespace Axel {

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: m_MouseX(x), m_MouseY(y) {}

		inline float GetX() const { return m_MouseX; };
		inline float GetY() const { return m_MouseY; };

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_MouseX;
		float m_MouseY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float offsetX, float offsetY)
			: m_OffsetX(offsetX), m_OffsetY(offsetY) {}

		inline float GetXOffset() const { return m_OffsetX; };
		inline float GetYOffset() const { return m_OffsetY; };

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_OffsetX;
		float m_OffsetY;
	};

	class MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return m_Button; };
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		MouseButtonEvent(int button)
			: m_Button(button) {}

		int m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}