#pragma once

#include "Hazel/Windows.h"

#include <GLFW/glfw3.h>

namespace Hazel {

	class HAZEL_API WindowsWindow :public Window {
	public:
		WindowsWindow(const WindowProps& props);

		virtual ~WindowsWindow();

		// 每帧更新，交换opengl帧缓存
		void OnUpdate() override;
		unsigned int GetWidth() const override {return m_Data.Width;}
		unsigned int GetHeight() const override { return m_Data.Height; }

		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	private:
		// 这个函数没有也要是虚函数...这个类应该已经到底了呀
		// 可能是不同的实现方式，例如GLFW以及Win32
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		// 嵌套类，防止外界构建
		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			EventCallbackFn EventCallback;
		};
		WindowData m_Data;
	};

}