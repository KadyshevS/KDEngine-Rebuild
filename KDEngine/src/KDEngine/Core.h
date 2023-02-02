#pragma once

#ifdef KD_PLATFORM_WINDOWS
	#ifdef KD_BUILD_DLL
		#define KD_API __declspec(dllexport)
	#else
		#define KD_API __declspec(dllimport)
	#endif
#else
	#error KDEngine supports only Microsoft Windows
#endif

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)