#pragma once
#include "Log.h"
#include "Application.h"


extern XYZ::Application* CreateApplication();

int main(int argc, char** argv)
{
	XYZ::Logger::Init();
	auto app = CreateApplication();
	app->Run();
	delete app;
}
