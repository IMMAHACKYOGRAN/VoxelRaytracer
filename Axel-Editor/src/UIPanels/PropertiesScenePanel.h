#pragma once
#include <Axel.h>

class PropertiesScenePanel
{
public:
	PropertiesScenePanel();

	void Draw();

	void SetScene(std::shared_ptr<Axel::Scene> scene) { m_CurrentScene = scene; }
	void SetSelectedEntity(Axel::EntityId entity) { m_SelectedEntity = entity; }
	void SetEditorCamera(Axel::OrbitalCamera& cam) { m_EditorCamera = &cam; }
	Axel::EntityId GetSelectedEntity() const { return m_SelectedEntity; }
	bool IsEntitySelected() const { return m_IsEntitySelected; }

private:
	void DrawEntity(Axel::EntityId entity);
	void DrawAllComponents();

	template<typename T>
	void DrawComponent(const std::string& componentName, void(*func)(T&));
	
private:
	bool m_FocusRename = false;
	bool m_DeleteEntity = false;

	std::shared_ptr<Axel::Scene> m_CurrentScene;

	bool m_IsEntitySelected = false;
	Axel::EntityId m_SelectedEntity;

	Axel::OrbitalCamera* m_EditorCamera = nullptr;
};