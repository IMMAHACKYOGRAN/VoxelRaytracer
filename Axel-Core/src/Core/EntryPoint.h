#pragma once
#include "Core/Core.h"
#include "Core/Application.h"

extern Axel::Application* Axel::CreateApplication();

int main(int argc, char* argv[])
{
	Axel::Log::Init();

	auto app = Axel::CreateApplication();
	app->Run();

	delete app;
    return 0;
}