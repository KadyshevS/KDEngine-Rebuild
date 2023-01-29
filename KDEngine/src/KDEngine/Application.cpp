#include "Application.h"

#include <KDEngine/Log.h>
#include <KDEngine/ApplicationEvent.h>
#include <KDEngine/KeyEvent.h>

namespace KDE
{
	Application::Application() {}
	Application::~Application() {}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			KD_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			KD_TRACE(e);
		}

		while (true);
	}
}