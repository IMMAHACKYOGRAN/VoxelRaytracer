#include <Axel.h>

class ExampleLayer : public Axel::Layer
{
public:
};

class AxelEditor : public Axel::Application
{
public:
	AxelEditor()
	{
		PushLayer(new ExampleLayer());
	}

	~AxelEditor()
	{

	}
};

Axel::Application* Axel::CreateApplication()
{
	return new AxelEditor();
}