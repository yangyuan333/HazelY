#pragma once
#include "Core.h"
#include "Windows.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/LayerStack.h"

#include <memory>

namespace Hazel {
	
	// Application�ǵ����ģ�һ��exeֻ��һ��application
	class HAZEL_API Application {
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlayer(Layer* layer);

	private:
		bool OnWindowClose(WindowCloseEvent& e);
	private:
		// �����Զ�����
		// һ������ֻ����һ��Application�����һ��
		// �Ȳ��漰�ര��ϵͳ
		std::unique_ptr<Window> m_Window;
		bool m_Running;
		LayerStack m_LayerStack;
	};

	// �����������ⲿ���ã���ʵ�ʲ��ǵģ���Ϊ���.h�ļ��ᱻinclude��sandbox��
	// ���������������sandbox���涨���
	Application* CreateApplication();
}