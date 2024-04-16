#pragma once
#include <GLFW/glfw3.h>

#include "Events/Event.h"

namespace Axel
{
	struct WindowSpecification
	{
		std::string Name;
		uint32_t Width;
		uint32_t Height;

		WindowSpecification(const std::string& title = "Axel App", uint32_t width = 1280, uint32_t height = 720)
			: Name(title), Width(width), Height(height)
		{
		}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window(const WindowSpecification& spec = WindowSpecification());
		~Window();

		void OnUpdate();

		inline uint32_t GetWidth() const { return m_Data.Width; };
		inline uint32_t GetHeight() const { return m_Data.Height; };

		inline void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallBack = callback; };
		void SetVSync(bool enabled);
		bool IsVSync() const;

		inline void* GetNativeWindow() const { return m_Window; };
	private:
		void Init(const WindowSpecification& spec);
		void Shutdown();
	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Name;
			uint32_t Width, Height;
			bool VSync;

			EventCallbackFn EventCallBack;
		};

		WindowData m_Data;
	};
}