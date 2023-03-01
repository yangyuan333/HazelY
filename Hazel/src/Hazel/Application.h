#pragma once
#include "Core.h"

namespace Hazel {

	class HAZEL_API Application {
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	// 看似是用以外部调用，但实际不是的，因为这个.h文件会被include到sandbox上
	// 而这个函数就是在sandbox里面定义的
	Application* CreateApplication();
}