#include "pch.h"

#include "Core/Input.h"
#include "Core/Application.h"
#include <GLFW/glfw3.h>

namespace Axel
{
	bool Input::IsKeyDown(KeyCode key)
	{
		GLFWwindow* window = Application::Get().GetWindow().GetNativeWindow();
		return glfwGetKey(window, (int)key) == GLFW_PRESS;
	}

	bool Input::IsMouseButtonDown(MouseButton button)
	{
		GLFWwindow* window = Application::Get().GetWindow().GetNativeWindow();
		return glfwGetMouseButton(window, (int)button) == GLFW_PRESS;
	}

	const glm::vec2 Input::GetMousePosition()
	{
		GLFWwindow* window = Application::Get().GetWindow().GetNativeWindow();
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return { (float)x, (float)y };
	}

	void Input::SetCursorMode(CursorMode mode)
	{
		GLFWwindow* window = Application::Get().GetWindow().GetNativeWindow();
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL + (int)mode);
	}
}