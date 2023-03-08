#pragma once

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"

namespace Hazel {
	// 他为什么没有使用 HAZEL_API呢？他不需要被第三方项目调用吗
	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Hazel Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			:Title(title), Width(width), Height(height) {}
	};

	class HAZEL_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		// 垂直同步---学习了解一下
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		// 静态没有虚函数的概念，没有this
		// 同时这个类是纯虚类，因此不会被实例化，所以只声明，留给子类定义是ok的
		// 函数本来就具有这种性质---可以只被声明，只有被调用时才会去link
		static Window* Create(const WindowProps& props = WindowProps());
	};

}