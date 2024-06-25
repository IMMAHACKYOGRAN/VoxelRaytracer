#include <string>
#include "PropertiesScenePanel.h"

PropertiesScenePanel::PropertiesScenePanel()
{
}

void PropertiesScenePanel::Draw()
{
	ImGui::Begin("Scene");

	for (const auto& e : m_CurrentScene->GetEntitiesWith<Axel::NameComponent>())
		DrawEntity(e);

	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		m_IsEntitySelected = false;

	ImGui::End();

	ImGui::Begin("Properties");

	if (m_IsEntitySelected)
		DrawComponents();

	ImGui::End();
}

void PropertiesScenePanel::DrawEntity(Axel::EntityId entity)
{
	ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity && m_IsEntitySelected) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	bool opened = ImGui::TreeNodeEx(m_CurrentScene->GetComponent<Axel::NameComponent>(entity).Name.c_str(), flags);
	if (ImGui::IsItemClicked()) 
	{
		m_SelectedEntity = entity;
		m_IsEntitySelected = true;
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

void PropertiesScenePanel::DrawComponents()
{
	auto& name = m_CurrentScene->GetComponent<Axel::NameComponent>(m_SelectedEntity).Name;

	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	strncpy_s(buffer, sizeof(buffer), name.c_str(), sizeof(buffer));

	if (ImGui::InputText("##name", buffer, sizeof(buffer)))
	{
		name = std::string(buffer);
	}

	ImGuiStyle& style = ImGui::GetStyle();

	float size = ImGui::CalcTextSize("Add Component").x + style.FramePadding.x * 2.0f;
	float avail = ImGui::GetContentRegionAvail().x;

	float off = (avail - size) / 2;
	float pos = ImGui::GetCursorPosX();
	ImGui::SetCursorPosX(pos + off);

	if (ImGui::Button("Add Component"))
		ImGui::OpenPopup("AddComponent");

	ImGui::SetCursorPosX(pos);

	if (ImGui::BeginPopup("AddComponent"))
	{
		ImGui::Text("Boo");

		ImGui::EndPopup();
	}

	//ImGuiTreeNodeFlags_DefaultOpen
}