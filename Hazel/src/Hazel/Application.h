#pragma once
#include "Core.h"
#include "Windows.h"
#include <memory>

namespace Hazel {
	
	// Application是单例的，一个exe只有一个application
	class HAZEL_API Application {
	public:
		Application();
		virtual ~Application();
		void Run();
	private:
		// 便于自动销毁
		// 一个窗口只会与一个Application结合在一起
		std::unique_ptr<Window> m_Window;
		bool m_Running;
	};

	// 看似是用以外部调用，但实际不是的，因为这个.h文件会被include到sandbox上
	// 而这个函数就是在sandbox里面定义的
	Application* CreateApplication();
}