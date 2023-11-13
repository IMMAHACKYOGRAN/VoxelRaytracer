#pragma once
#include <imgui.h>

class ImGuiLayer
{
public:
	ImGuiLayer();
	~ImGuiLayer();

	void Init();
	void Shutdown();

	void Begin();
	void End();

};