#include "pch.h"
#include "Window.h"

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

#include <glad/glad.h>

namespace Axel
{
	static bool s_GLFWInitialised = false;

	static void GLFWErrorCallback(int error_code, const char* description)
	{
		AX_ERROR("{1} GLFW Error Code: {0}", error_code, description);
	}

	Window::Window(const WindowSpecification& spec)
	{
		Init(spec);
	}

	Window::~Window()
	{
		Shutdown();
	}

	void Window::Init(const WindowSpecification& spec)
	{
		m_Data.Name = spec.Name;
		m_Data.Width = spec.Width;
		m_Data.Height = spec.Height;

		if (!s_GLFWInitialised)
		{
			int success = glfwInit();
			AX_ASSERT(success, "Could not intialise GLFW");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialised = true;
		}

		m_Window = glfwCreateWindow((int)spec.Width, (int)spec.Height, m_Data.Name.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		AX_ASSERT(status, "Could not intialise Glad");
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent e(width, height);
				data.EventCallBack(e);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent e;
				data.EventCallBack(e);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent e(key, 0);
					data.EventCallBack(e);
					break;
				}

				case GLFW_RELEASE:
				{
					KeyReleasedEvent e(key);
					data.EventCallBack(e);
					break;
				}

				case GLFW_REPEAT:
				{
					KeyPressedEvent e(key, 1);
					data.EventCallBack(e);
					break;
				}
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t key)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyTypedEvent e(key);
				data.EventCallBack(e);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent e(button);
					data.EventCallBack(e);
					break;
				}

				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent e(button);
					data.EventCallBack(e);
					break;
				}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent e((float)xoffset, (float)yoffset);
				data.EventCallBack(e);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent e((float)xPos, (float)yPos);
				data.EventCallBack(e);
			});
	}

	void Window::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void Window::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool Window::IsVSync() const
	{
		return m_Data.VSync;
	}

	void Window::SetIcon(int width, int height, uint8_t* pixels)
	{
		GLFWimage img[1]{width, height, pixels};
		glfwSetWindowIcon(m_Window, 1, img);
	}
}