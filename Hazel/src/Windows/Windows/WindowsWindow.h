#pragma once

#include "Hazel/Windows.h"

#include <GLFW/glfw3.h>

namespace Hazel {

	class HAZEL_API WindowsWindow :public Window {
	public:
		WindowsWindow(const WindowProps& props);

		virtual ~WindowsWindow();

		// ÿ֡���£�����opengl֡����
		void OnUpdate() override;
		unsigned int GetWidth() const override {return m_Data.Width;}
		unsigned int GetHeight() const override { return m_Data.Height; }

		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	private:
		// �������û��ҲҪ���麯��...�����Ӧ���Ѿ�������ѽ
		// �����ǲ�ͬ��ʵ�ַ�ʽ������GLFW�Լ�Win32
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		// Ƕ���࣬��ֹ��繹��
		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			EventCallbackFn EventCallback;
		};
		WindowData m_Data;
	};

}