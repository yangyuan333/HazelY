#pragma once

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"

namespace Hazel {
	// ��Ϊʲôû��ʹ�� HAZEL_API�أ�������Ҫ����������Ŀ������
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
		// ��ֱͬ��---ѧϰ�˽�һ��
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		// ��̬û���麯���ĸ��û��this
		// ͬʱ������Ǵ����࣬��˲��ᱻʵ����������ֻ�������������ඨ����ok��
		// ���������;�����������---����ֻ��������ֻ�б�����ʱ�Ż�ȥlink
		static Window* Create(const WindowProps& props = WindowProps());
	};

}