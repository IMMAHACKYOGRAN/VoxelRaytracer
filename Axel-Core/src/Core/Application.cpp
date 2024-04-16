#include "pch.h"

#include "Application.h"

namespace Axel
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		AX_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(new Window());
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	}

	Application::~Application()
	{
	}

	void Application::Run() 
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime();
			float timeStep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimised)
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timeStep);
			}

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

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
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

		m_Minimised = false;
		//Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}