#pragma once
#include "Application.h"
#include <stdio.h>

#ifdef KD_PLATFORM_WINDOWS

extern KDE::Application* KDE::CreateApplication();

int main(int argc, char** argv)
{
	printf("KDEngine started successfully\n");

	auto app = KDE::CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#endif