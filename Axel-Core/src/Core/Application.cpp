#include "pch.h"

#include "Application.h"
#include "Renderer/Renderer.h"

namespace Axel
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& spec)
		: m_Spec(spec)
	{
		AX_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		WindowSpecification wspec;
		wspec.Name = spec.Name;
		wspec.Width = spec.Width;
		wspec.Height = spec.Height;

		m_Window = std::unique_ptr<Window>(new Window(wspec));
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
	}

	void Application::Run() 
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime();
			float deltaTime = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimised)
			{
				for (Layer* layer : m_LayerStack.GetLayers())
					layer->OnUpdate(deltaTime);
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack.GetLayers())
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}
	
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));

		for (Layer* layer : m_LayerStack.GetLayers())
		{
			layer->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::SetIcon(uint8_t* pixels, int width, int height)
	{
		m_Window->SetIcon(width, height, pixels);
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimised = true;
			return false;
		}

		Renderer::SetViewport(0, 0, e.GetWidth(), e.GetHeight());

		m_Minimised = false;

		return false;
	}
}