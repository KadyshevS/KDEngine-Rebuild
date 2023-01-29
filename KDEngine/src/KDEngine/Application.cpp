#include "Application.h"

#include <KDEngine/ApplicationEvent.h>
#include <KDEngine/Log.h>

namespace KDE
{
	Application::Application() {}
	Application::~Application() {}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		KD_TRACE(e);

		while (true);
	}
}