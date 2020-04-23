#pragma once
#include "Logger.h"
#include "XYZ/ECS/ECSManager.h"
#include "Application.h"


extern XYZ::Application* CreateApplication();

int main(int argc, char** argv)
{
	XYZ::Logger::Init();
	XYZ::ECSManager::Init();
	auto app = CreateApplication();
	app->Run();
	delete app;
}
