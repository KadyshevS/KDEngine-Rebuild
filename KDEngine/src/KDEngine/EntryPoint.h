#pragma once

#include <KDEngine.h>

#ifdef KD_PLATFORM_WINDOWS

extern KDE::Application* KDE::CreateApplication();

int main(int argc, char** argv)
{
	KDE::Log::Init();
	auto app = KDE::CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#endif