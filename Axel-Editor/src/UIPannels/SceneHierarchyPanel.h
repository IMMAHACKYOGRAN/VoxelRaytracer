#pragma once
#include <imgui.h>

class SceneHeirarchyPanel
{
public:
	SceneHeirarchyPanel();

	void OnImGuiDraw();

private:
	void DrawEntity(std::string name);
	
private:
	std::string m_SelectionContext; //sould be entity id
};