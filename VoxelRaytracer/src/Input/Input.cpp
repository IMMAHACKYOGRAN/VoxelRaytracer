#include "Input.h"

#include <GLFW/glfw3.h>

bool Input::IsKeyDown(KeyCode keycode)
{
	GLFWwindow* window = Application::Get().GetWindow();
	int state = glfwGetKey(window, (int)keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonDown(MouseButton button)
{
	GLFWwindow* window = Application::Get().GetWindow();
	int state = glfwGetMouseButton(window, (int)button);
	return state == GLFW_PRESS;
}

glm::vec2 Input::GetMousePosition()
{
	GLFWwindow* window = Application::Get().GetWindow();

	double x, y;
	glfwGetCursorPos(window, &x, &y);
	return { (float)x, (float)y };
}

void Input::SetCursorMode(CursorMode mode)
{
	GLFWwindow* window = Application::Get().GetWindow();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL + (int)mode);
}