#pragma once

#include <memory>

#include <memory>

#ifdef _WIN32
/* Windows x64/x86 */
#ifdef _WIN64
	/* Windows x64  */
#define KD_PLATFORM_WINDOWS
#else
	/* Windows x86 */
#error "x86 Builds are not supported."
#endif
#elif defined(__APPLE__) || defined(__MACH__)
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR == 1
#error "IOS simulator is not supported."
#elif TARGET_OS_IPHONE == 1
#define KD_PLATFORM_IOS
#error "IOS is not supported."
#elif TARGET_OS_MAC == 1
#define KD_PLATFORM_MACOS
#error "MacOS is not supported."
#else
#error "Unknown Apple platform."
#endif
#elif defined(__ANDROID__)
#define KD_PLATFORM_ANDROID
#error "Android is not supported."
#elif defined(__linux__)
#define KD_PLATFORM_LINUX
#error "Linux is not supported."
#else
#error "Unknown platform."
#endif

//////////	DLL	 //////////////////////////////////////////////////////////////////////////
#ifdef KD_PLATFORM_WINDOWS
#if KD_DYNAMIC_LINK
#ifdef KD_BUILD_DLL
#define KD_API __declspec(dllexport)
#else
#define KD_API __declspec(dllimport)
#endif
#else
#define KD_API
#endif
#else
#error KDEngine only supports Windows.
#endif //

#ifdef KD_DEBUG
#define KD_ENABLE_ASSERTS
#endif

#ifdef KD_ENABLE_ASSERTS
#define KD_ASSERT(x, ...) { if(!(x)) { KD_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define KD_CORE_ASSERT(x, ...) { if(!(x)) { KD_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define KD_ASSERT(x, ...)
#define KD_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define KD_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace KDE
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> MakeScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> MakeRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}