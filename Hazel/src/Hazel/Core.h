#pragma once

#ifdef HZ_PLATFORM_WINDOWS
#ifdef HZ_DYNAMIC_LINK
	#ifdef HZ_BUILD_DLL
		#define HAZEL_API __declspec(dllexport)
	#else
		#define HAZEL_API __declspec(dllimport)
	#endif
#else
#define HAZEL_API 
#endif
#else
#error Hazel only support Windows!
#endif

#define M_PI 3.14159f

// 位运算操作符宏定义
#define BIT(x) (1 << x)

#ifdef HZ_DEBUG
	#define HZ_ENABLE_ASSERT
#endif

// Assert
#ifdef HZ_ENABLE_ASSERT
	#define HZ_ASSERT(x, ...) { if(!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define HZ_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define HZ_ASSERT(x, ...)
	#define HZ_CORE_ASSERT(x, ...)
#endif

#define HZ_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)