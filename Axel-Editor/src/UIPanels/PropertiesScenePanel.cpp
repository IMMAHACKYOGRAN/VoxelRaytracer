#include <string>
#include "PropertiesScenePanel.h"
#include <gtc/type_ptr.hpp>
#include <imgui_internal.h>

PropertiesScenePanel::PropertiesScenePanel()
{
}

void PropertiesScenePanel::Draw()
{
	m_FocusRename = false;

	ImGui::Begin("Scene");

	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::MenuItem("Add Entity")) 
		{
			m_SelectedEntity = (Axel::EntityId)m_CurrentScene->CreateEntity(); 
			m_IsEntitySelected = true;
			m_FocusRename = true;
		}
		ImGui::EndPopup();
	}

	if (m_DeleteEntity)
	{
		m_CurrentScene->RemoveEntity(m_SelectedEntity);
		m_IsEntitySelected = false;
		m_DeleteEntity = false;
	}

	for (const auto& e : m_CurrentScene->GetEntitiesWith<Axel::NameComponent>())
		DrawEntity(e);

	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		m_IsEntitySelected = false;

	ImGui::End();

	ImGui::Begin("Properties");

	if (m_IsEntitySelected)
		DrawAllComponents();

	ImGui::End();
}

void PropertiesScenePanel::DrawEntity(Axel::EntityId entity)
{
	ImGuiTreeNodeFlags flags = (m_SelectedEntity == entity && m_IsEntitySelected) ? ImGuiTreeNodeFlags_Selected : 0;
	flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	bool opened = ImGui::TreeNodeEx((void*)entity, flags, m_CurrentScene->GetComponent<Axel::NameComponent>(entity).Name.c_str());
	if (ImGui::IsItemClicked())
	{
		m_SelectedEntity = entity;
		m_IsEntitySelected = true;

		if (ImGui::IsMouseDoubleClicked(0))
		{
			m_EditorCamera->SetFocalPoint(m_CurrentScene->GetComponent<Axel::TransformComponent>(m_SelectedEntity).Translation);
			m_EditorCamera->SetDistance(10.0f);
		}
	}

	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Copy")) {}
		if (ImGui::MenuItem("Paste")) {}
		ImGui::Separator();
		if (ImGui::MenuItem("Focus"))
			m_EditorCamera->SetFocalPoint(m_CurrentScene->GetComponent<Axel::TransformComponent>(m_SelectedEntity).Translation);
		ImGui::Separator();
		if (ImGui::MenuItem("Rename")) { m_FocusRename = true; }
		if (ImGui::MenuItem("Delete")) { m_DeleteEntity = true; }
		ImGui::EndPopup();
	}

	if (opened)
		ImGui::TreePop();
}

template<typename T>
void PropertiesScenePanel::DrawComponent(const std::string& componentName, void(*func)(T&))
{
	ImGuiStyle& style = ImGui::GetStyle();

	bool shouldremove = false;

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap;
	if (m_CurrentScene->HasComponent<T>(m_SelectedEntity))
	{
		auto& comp = m_CurrentScene->GetComponent<T>(m_SelectedEntity);
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, componentName.c_str());
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Remove component"))
				shouldremove = true;
			ImGui::EndPopup();
		}

		if (open)
		{
			func(comp);
			ImGui::TreePop();
		}

		if (shouldremove) // Must do at the end to avoid rendering UI for deleted component
			m_CurrentScene->RemoveComponent<T>(m_SelectedEntity);
	}
}

static void DrawVec3(const std::string& label, glm::vec3& values, float defaultvalue = 0.0f)
{
	ImGui::PushID(label.c_str());

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	if (ImGui::Button("X", buttonSize)) { values.x = defaultvalue; }

	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	if (ImGui::Button("Y", buttonSize)) { values.y = defaultvalue; }

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	if (ImGui::Button("Z", buttonSize)) { values.z = defaultvalue; }

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar(2);
	ImGui::PopID();
}

void PropertiesScenePanel::DrawAllComponents()
{
	ImGuiStyle& style = ImGui::GetStyle();

	auto& name = m_CurrentScene->GetComponent<Axel::NameComponent>(m_SelectedEntity).Name;

	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	strncpy_s(buffer, sizeof(buffer), name.c_str(), sizeof(buffer));
	ImGui::PushItemWidth(-1);
	if (m_FocusRename) { ImGui::SetKeyboardFocusHere(0); }
	if (ImGui::InputText("##name", buffer, sizeof(buffer)))
	{
		name = std::string(buffer);
	}
	ImGui::PopItemWidth();

	DrawComponent<Axel::TransformComponent>("Transform", [](auto& comp) 
		{
			DrawVec3("Translation", comp.Translation);
			glm::vec3 rot = glm::degrees(comp.Rotation);
			DrawVec3("Rotation", rot);
			comp.Rotation = glm::radians(rot);
			DrawVec3("Scale", comp.Scale, 1.0f);
			ImGui::Spacing();
		});

	DrawComponent<Axel::CameraComponent>("Camera", [](auto& comp)
		{
			float fov = comp.Cam.GetFOV();
			ImGui::DragFloat("Fov", &fov, 1.0f, 1.0f, 90.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
			comp.Cam.SetFOV(fov);

			float ndist = comp.Cam.GetNearClip();
			ImGui::DragFloat("Near Clip", &ndist, 1.0f, 0.01f, 1000.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
			comp.Cam.SetNearClip(ndist);

			float fdist = comp.Cam.GetFarClip();
			ImGui::DragFloat("Far Clip", &fdist, 1.0f, 0.02f, 10000.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
			comp.Cam.SetFarClip(fdist);
		});

	DrawComponent<Axel::ScriptComponent>("Script", [](auto& comp)
		{
			
		});
	ImGui::Separator();

	ImGui::Dummy({0, 10});

	float size = ImGui::CalcTextSize("Add Component").x + style.FramePadding.x * 2.0f;
	float avail = ImGui::GetContentRegionAvail().x;

	float off = (avail - size) / 2;
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

	if (ImGui::Button("Add Component"))
		ImGui::OpenPopup("AddComponent");

	if (ImGui::BeginPopup("AddComponent"))
	{
		if (ImGui::Button("Camera Component")) 
		{
			if (!m_CurrentScene->HasComponent<Axel::CameraComponent>(m_SelectedEntity))
			{
				m_CurrentScene->AddComponent<Axel::CameraComponent>(m_SelectedEntity);
				m_CurrentScene->SetMainCameraEntity(m_SelectedEntity);
			}
		}

		if (ImGui::Button("Script Component"))
		{
			if (!m_CurrentScene->HasComponent<Axel::ScriptComponent>(m_SelectedEntity))
				m_CurrentScene->AddComponent<Axel::ScriptComponent>(m_SelectedEntity);
		}

		ImGui::EndPopup();
	}
}