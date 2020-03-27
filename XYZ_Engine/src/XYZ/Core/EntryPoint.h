#pragma once
#include "Log.h"
#include "XYZ/ECS/ECSManager.h"
#include "Application.h"


extern XYZ::Application* CreateApplication();

int main(int argc, char** argv)
{
	XYZ::Logger::Init();
	XYZ::ECSManager::Get()->Init();
	auto app = CreateApplication();
	app->Run();
	delete app;
}
