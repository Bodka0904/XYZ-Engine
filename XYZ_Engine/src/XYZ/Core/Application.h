#pragma once
#include "Layer.h"
#include "Window.h"
#include "Event/EventManager.h"

#include <vector>

namespace XYZ {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void PushLayer(Layer* layer);
		void PushBackLayer(Layer* backlayer);
		void PopLayer(Layer* layer);

		Window& GetWin() { return *m_Window; }
		inline static Application& Get() { return *s_Application; }

		static Application* CreateApplication();
	private:
		void OnWindowClose(event_ptr event);
		void OnWindowResize(event_ptr event);
	private:
		HandlerID m_WindowResize;

		std::unique_ptr<Window> m_Window;
		std::vector<Layer*> m_Layers;

		unsigned int m_LayoutInsertIndex = 0;
		float m_LastFrameTime = 0.0f;
		
		static Application* s_Application;
	};

}