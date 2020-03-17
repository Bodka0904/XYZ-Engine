#pragma once
#include "Layer.h"
#include "Window.h"

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

		void OnEvent(Event& e);

		Window& GetWin() { return *m_Window; }

		inline static Application& Get() { return *s_Application; }

		static Application* CreateApplication();
	private:
		std::unique_ptr<Window> m_Window;
		unsigned int m_LayoutInsertIndex = 0;
		float m_LastFrameTime = 0.0f;

		std::vector<Layer*> m_Layers;
		static Application* s_Application;
	};

}