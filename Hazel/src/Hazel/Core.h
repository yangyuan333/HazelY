#pragma once

#ifdef HZ_PLATFORM_WINDOWS
#ifdef HZ_BUILD_DLL
#define HAZEL_API __declspec(dllexport)
#else
#define HAZEL_API __declspec(dllimport)
#endif
#else
#error Hazel only support Windows!
#endif

// 位运算操作符宏定义
#define BIT(x) (1 << x)