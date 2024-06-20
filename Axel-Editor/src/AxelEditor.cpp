#include <Axel.h>
#include <Core/EntryPoint.h>

#include "EditorLayer.h"

class AxelEditor : public Axel::Application
{
public:
	AxelEditor(const Axel::ApplicationSpecification& spec)
		: Axel::Application(spec)
	{
		PushLayer(new EditorLayer());
	}
};

Axel::Application* Axel::CreateApplication()
{
	Axel::ApplicationSpecification spec;
	spec.Name = "Axel Editor";

	return new AxelEditor(spec);
}