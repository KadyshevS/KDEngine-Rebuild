#pragma once

#include "Core.h"

#include "Window.h"
#include "KDEngine/LayerStack.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace KDE
{
	class KD_API Application
	{
		friend class WindowsWindow;

	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
		bool m_Running = true;
	};

//	Need to be declared in client region
	Application* CreateApplication();
}
