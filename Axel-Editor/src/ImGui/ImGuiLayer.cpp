#include "ImGuiLayer.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../Application.h"


ImGuiLayer::ImGuiLayer()
{
}

ImGuiLayer::~ImGuiLayer()
{
}

void ImGuiLayer::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();

	if (io.ConfigFlags)
	{
		style.WindowRounding = 6.0f;
		style.FrameRounding = 3.0f;
		style.PopupRounding = 6.0f;
		style.ScrollbarRounding = 12.0f;
		style.GrabRounding = 3.0f;
		style.TabRounding = 6.0f;
	}

	Application& app = Application::Get();
	GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow());

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::Begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::End()
{
	ImGuiIO& io = ImGui::GetIO();
	Application& app = Application::Get();
	io.DisplaySize = ImVec2((float)app.GetWidth(), (float)app.GetHeight());

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}
