#include <XYZ.h>
#include <XYZ/Core/EntryPoint.h>

#include "TestLayer.h"

class Sandbox : public XYZ::Application
{
public:
	Sandbox()
	{
		PushLayer(new TestLayer());
	}

	~Sandbox()
	{

	}
};

XYZ::Application* CreateApplication()
{
	return new Sandbox();
}
