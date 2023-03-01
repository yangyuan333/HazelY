#pragma once

#include <memory>
#include "Core.h"
#include "spdlog/spdlog.h"

namespace Hazel {
	// 这个类相当于是一个静态类，没有用单例模式实现，而是完全用的static，保证全局唯一
	class HAZEL_API Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger> getCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger> getClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#define HZ_CORE_FATAL(...)	::Hazel::Log::getCoreLogger()->fatal(__VA_ARGS__)
#define HZ_CORE_ERROR(...)	::Hazel::Log::getCoreLogger()->error(__VA_ARGS__)
#define HZ_CORE_WARN(...)	::Hazel::Log::getCoreLogger()->warn(__VA_ARGS__)
#define HZ_CORE_INFO(...)	::Hazel::Log::getCoreLogger()->info(__VA_ARGS__)
#define HZ_CORE_TRACE(...)	::Hazel::Log::getCoreLogger()->trace(__VA_ARGS__)

#define HZ_FATAL(...)		::Hazel::Log::getClientLogger()->fatal(__VA_ARGS__)
#define HZ_ERROR(...)		::Hazel::Log::getClientLogger()->error(__VA_ARGS__)
#define HZ_WARN(...)		::Hazel::Log::getClientLogger()->warn(__VA_ARGS__)
#define HZ_INFO(...)		::Hazel::Log::getClientLogger()->info(__VA_ARGS__)
#define HZ_TRACE(...)		::Hazel::Log::getClientLogger()->trace(__VA_ARGS__)