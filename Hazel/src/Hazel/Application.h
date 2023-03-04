#pragma once
#include "Core.h"
#include "Windows.h"
#include <memory>

namespace Hazel {
	
	// Application�ǵ����ģ�һ��exeֻ��һ��application
	class HAZEL_API Application {
	public:
		Application();
		virtual ~Application();
		void Run();
	private:
		// �����Զ�����
		// һ������ֻ����һ��Application�����һ��
		std::unique_ptr<Window> m_Window;
		bool m_Running;
	};

	// �����������ⲿ���ã���ʵ�ʲ��ǵģ���Ϊ���.h�ļ��ᱻinclude��sandbox��
	// ���������������sandbox���涨���
	Application* CreateApplication();
}