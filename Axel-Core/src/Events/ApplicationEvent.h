#pragma once

#include "Event.h"

namespace Axel {

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32_t width, uint32_t height)
			: m_Width(width), m_Height(height) {}
			
		inline uint32_t GetHeight() const { return m_Height; };
		inline uint32_t GetWidth() const { return m_Width; };

		static EventType GetStaticType() { return EventType::WindowResize; }
		virtual EventType GetEventType() const override { return GetStaticType(); }

		virtual int GetCategoryFlags() const override { return EventCategoryApplication; }
	private:
		uint32_t m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		static EventType GetStaticType() { return EventType::WindowClose; }
		virtual EventType GetEventType() const override { return GetStaticType(); }

		virtual int GetCategoryFlags() const override { return EventCategoryApplication; }
	};
}