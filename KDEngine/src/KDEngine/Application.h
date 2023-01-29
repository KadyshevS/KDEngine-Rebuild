#pragma once

#include "Core.h"
#include <KDEngine/Event.h>

namespace KDE
{
	class KD_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

//	Need to be declared in client region
	Application* CreateApplication();
}
