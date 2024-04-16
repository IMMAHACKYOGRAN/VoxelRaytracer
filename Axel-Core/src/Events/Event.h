#pragma once
#include "pch.h"

#include "Core/Core.h"

namespace Axel {

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= (1 << 0),
		EventCategoryInput			= (1 << 1),
		EventCategoryKeyboard		= (1 << 2),
		EventCategoryMouse			= (1 << 3),
		EventCategoryMouseButton	= (1 << 4),
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event
	{
	public:
		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual int GetCategoryFlags() const = 0;

		inline bool IsInCategory(EventCategory category) const { return GetCategoryFlags() & category; }
	};


	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& e)
			: m_Event(e) {}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};
}