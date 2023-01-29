#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

namespace KDE
{
	class KD_API Application
	{
		friend class WindowsWindow;

	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

//	Need to be declared in client region
	Application* CreateApplication();
}
