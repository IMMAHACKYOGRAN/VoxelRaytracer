#pragma once

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

#include "Core/LayerStack.h"
#include "Core/Window.h"
#include "Renderer/Texture.h"

#include "ImGui/ImGuiLayer.h"

#include "ECS/Entity.h"

namespace Axel
{
	struct ApplicationSpecification
	{
		std::string Name;
		uint32_t Width;
		uint32_t Height;

		ApplicationSpecification(const std::string& name = "Axel App", uint32_t width = 1280, uint32_t height = 720)
			: Name(name), Width(width), Height(height)
		{
		}
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& spec);
		virtual ~Application();

		void Run();
		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		void SetIcon(uint8_t* pixels, int width, int height);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
		inline ImGuiLayer* GetUILayer() { return m_ImGuiLayer; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		ApplicationSpecification m_Spec;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimised = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}