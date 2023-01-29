#pragma once

#include "Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace KDE
{
	class KD_API Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; };
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; };

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};	
}

#define KD_CORE_TRACE(...)	::KDE::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define KD_CORE_INFO(...)	::KDE::Log::GetCoreLogger()->info(__VA_ARGS__)
#define KD_CORE_WARN(...)	::KDE::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define KD_CORE_ERROR(...)	::KDE::Log::GetCoreLogger()->error(__VA_ARGS__)
#define KD_CORE_FATAL(...)	::KDE::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define KD_TRACE(...)		::KDE::Log::GetClientLogger()->trace(__VA_ARGS__)
#define KD_INFO(...)		::KDE::Log::GetClientLogger()->info(__VA_ARGS__)
#define KD_WARN(...)		::KDE::Log::GetClientLogger()->warn(__VA_ARGS__)
#define KD_ERROR(...)		::KDE::Log::GetClientLogger()->error(__VA_ARGS__)
#define KD_FATAL(...)		::KDE::Log::GetClientLogger()->critical(__VA_ARGS__)

