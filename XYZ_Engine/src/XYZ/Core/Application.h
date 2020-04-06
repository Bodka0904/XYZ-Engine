#pragma once
#include "Layer.h"
#include "Window.h"
#include "Event/Event.h"

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

		void OnEvent(event_ptr e);

		Window& GetWin() { return *m_Window; }

		inline static Application& Get() { return *s_Application; }

		static Application* CreateApplication();
	private:
		void OnWindowClose(event_ptr e);
		void OnWindowResize(event_ptr e);
		void OnWindowResize2(event_ptr e);
	private:
		std::unique_ptr<Window> m_Window;
		unsigned int m_LayoutInsertIndex = 0;
		float m_LastFrameTime = 0.0f;

		std::vector<Layer*> m_Layers;
		static Application* s_Application;
	};

}