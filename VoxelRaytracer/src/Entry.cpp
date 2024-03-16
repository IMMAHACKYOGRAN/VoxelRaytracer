#include "Application.h"

int main(int argc, char* argv[])
{	
	ApplicationSpecification spec;
	spec.Name = "Axel";

	Application* app = new Application(spec);
	app->Run();

	delete app;
    return 0;
}