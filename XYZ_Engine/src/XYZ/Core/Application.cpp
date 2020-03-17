#include "stdafx.h"
#include "Application.h"


#include "XYZ/Timer.h"
#include "XYZ/Renderer/Renderer.h"

#include <GLFW/glfw3.h>


namespace XYZ {
	Application* Application::s_Application = nullptr;

	Application::Application()
	{
		s_Application = this;

		m_Window = Window::Create();
		m_Window->SetVSync(false);
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		// Push default layers
	}

	Application::~Application()
	{
		for (Layer* layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void Application::Run()
	{
		while (!m_Window->IsClosed())
		{
			float time = (float)glfwGetTime();
			float timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer* layer : m_Layers)
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
		m_Layers.emplace(m_Layers.begin() + m_LayoutInsertIndex, layer);
		m_LayoutInsertIndex++;
		layer->OnAttach();
	}

	void Application::PushBackLayer(Layer* backlayer)
	{
		m_Layers.emplace_back(backlayer);
		backlayer->OnAttach();
	}

	void Application::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayoutInsertIndex, layer);
		if (it != m_Layers.begin() + m_LayoutInsertIndex)
		{
			(*it)->OnDetach();

			m_Layers.erase(it);
			m_LayoutInsertIndex--;
		}
	}


	void Application::OnEvent(Event& event)
	{
		if (event.GetEventType() == EventType::WindowResized)
		{
			WindowResizeEvent& resize = (WindowResizeEvent&)event;
			Renderer::OnWindowResize(resize.GetWidth(), resize.GetHeight());
		}
		for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); ++it)
		{
			(*it)->OnEvent(event);
			if (event.IsHandled())
				break;
		}
	}

}