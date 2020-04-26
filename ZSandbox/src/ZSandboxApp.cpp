#include <XYZ.h>
#include <XYZ/Core/EntryPoint.h>

#include "GameLayer.h"

class Sandbox : public XYZ::Application
{
public:
	Sandbox()
	{
		PushLayer(new GameLayer());
	}

	~Sandbox()
	{

	}
};

XYZ::Application* CreateApplication()
{
	return new Sandbox();
}
