#pragma once
#include <imgui.h>
#include <string>

class PropertiesPanel
{
public:
	PropertiesPanel();

	void OnImGuiDraw();

private:
	void DrawComponent(std::string name, ImGuiTreeNodeFlags f);

private:
	std::string m_SelectionContext;
};