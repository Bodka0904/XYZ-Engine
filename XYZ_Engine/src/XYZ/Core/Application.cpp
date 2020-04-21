#include "stdafx.h"
#include "Application.h"


#include "XYZ/Timer.h"
#include "XYZ/Renderer/Renderer.h"


#include <GLFW/glfw3.h>

namespace XYZ {
	Application* Application::s_Application = nullptr;


	Application::Application()
	{
		Logger::Get().SetLogLevel(LogLevel::INFO | LogLevel::WARNING | LogLevel::ERR);
		//Logger::Get().SetLogLevel(LogLevel::NOLOG);
		s_Application = this;
		m_Running = true;

		m_Window = Window::Create();
		m_Window->SetVSync(false);

		m_GuiLayer = new GuiLayer();
		m_LayerStack.PushOverlay(m_GuiLayer);
		m_WindowResize = EventManager::Get().AddHandler(EventType::WindowResized, std::bind(&Application::OnWindowResize, this, std::placeholders::_1));
		m_WindowClose = EventManager::Get().AddHandler(EventType::WindowClosed, std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
		// Push default layers
	}

	Application::~Application()
	{
		
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime();
			float timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer* layer : m_LayerStack)
			{
				{
					//Stopwatch watch;
					layer->OnUpdate(timestep);
				}
			}
			m_Window->Update();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlayer)
	{
		m_LayerStack.PushOverlay(overlayer);		
	}

	void Application::PopLayer(Layer* layer)
	{
		m_LayerStack.PopLayer(layer);
	}

	void Application::Stop()
	{
		m_Running = false;
	}

	void Application::OnWindowClose(event_ptr event)
	{
		m_Running = false;
	}

	void Application::OnWindowResize(event_ptr event)
	{
		std::shared_ptr<WindowResizeEvent> resize = std::dynamic_pointer_cast<WindowResizeEvent>(event);
		Renderer::OnWindowResize(resize->GetWidth(), resize->GetHeight());
	}
}