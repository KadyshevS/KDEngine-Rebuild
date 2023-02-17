#pragma once

#include <memory>

#ifdef KD_PLATFORM_WINDOWS
	#ifdef KD_DYNAMIC_LINK
		#ifdef KD_BUILD_DLL
			#define KD_API __declspec(dllexport)
		#else
			#define KD_API __declspec(dllimport)
		#endif
	#else
		#define KD_API
	#endif
#else
	#error KDEngine supports only Microsoft Windows
#endif

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace KDE
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}