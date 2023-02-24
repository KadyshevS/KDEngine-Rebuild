#pragma once

#include <KDEngine.h>

#ifdef KD_PLATFORM_WINDOWS

extern KDE::Application* KDE::CreateApplication();

int main(int argc, char** argv)
{
	KDE::Log::Init();

	KD_PROFILE_BEGIN_SESSION("Startup", "KDProfile-Startup.json");
	auto app = KDE::CreateApplication();
	KD_PROFILE_END_SESSION();

	KD_PROFILE_BEGIN_SESSION("Runtime", "KDProfile-Runtime.json");
	app->Run();
	KD_PROFILE_END_SESSION();

	KD_PROFILE_BEGIN_SESSION("Shutdown", "KDProfile-Shutdown.json");
	delete app;
	KD_PROFILE_END_SESSION();

	return 0;
}

#endif