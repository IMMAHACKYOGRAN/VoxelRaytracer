#include <string>
#include "SceneHierarchyPanel.h"

SceneHeirarchyPanel::SceneHeirarchyPanel()
{
}

void SceneHeirarchyPanel::OnImGuiDraw()
{
	ImGui::Begin("Scene");

	DrawEntity("Cube");
	DrawEntity("Camera");
	DrawEntity("Directional Light");

	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		m_SelectionContext = "";

	ImGui::End();
}

void SceneHeirarchyPanel::DrawEntity(std::string name)
{
	ImGuiTreeNodeFlags flags = ((m_SelectionContext == name) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	bool opened = ImGui::TreeNodeEx(name.c_str(), flags);
	if (ImGui::IsItemClicked()) 
	{
		m_SelectionContext = name;
	}

	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Copy")) {}
		if (ImGui::MenuItem("Paste")) {}
		ImGui::Separator();
		if (ImGui::MenuItem("Rename")) {}
		if (ImGui::MenuItem("Delete")) {}
		ImGui::EndPopup();
	}

	if (opened)
		ImGui::TreePop();
}


