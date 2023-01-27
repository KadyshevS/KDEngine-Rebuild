#pragma once

#include <KDEngine.h>

#ifdef KD_PLATFORM_WINDOWS

extern KDE::Application* KDE::CreateApplication();

int main(int argc, char** argv)
{
	KDE::Log::Init();
	KD_WARN("Hello World!");

	unsigned int fds = 312312;
	KD_FATAL("Core error: {0}", fds);

	auto app = KDE::CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#endif