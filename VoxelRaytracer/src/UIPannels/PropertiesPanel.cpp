#include "PropertiesPanel.h"

PropertiesPanel::PropertiesPanel()
{
}

void PropertiesPanel::OnImGuiDraw()
{
    ImGui::Begin("Properties");
    
	DrawComponent("Transform" , ImGuiTreeNodeFlags_DefaultOpen);

    ImGui::End();
}

void PropertiesPanel::DrawComponent(std::string name, ImGuiTreeNodeFlags f)
{
	float p[3] = { 0,0,0 };
	float r[3] = { 0,0,0 };
	float s[3] = { 0,0,0 };

	if (ImGui::CollapsingHeader(name.c_str(), f))
	{
		ImGui::DragFloat3("Position", p);
		ImGui::DragFloat3("Rotation", r);
		ImGui::DragFloat3("Scale", s);
	}

	if (ImGui::IsItemClicked())
	{
		m_SelectionContext = name;
	}
}
