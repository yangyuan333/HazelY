#pragma once
#include "Core.h"

namespace Hazel {

	class HAZEL_API Application {
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	// �����������ⲿ���ã���ʵ�ʲ��ǵģ���Ϊ���.h�ļ��ᱻinclude��sandbox��
	// ���������������sandbox���涨���
	Application* CreateApplication();
}