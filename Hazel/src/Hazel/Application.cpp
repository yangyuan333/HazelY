#include "Application.h"

namespace Hazel {
	Application::Application() {
		// ������һ�����⣬��������Ļ���ô�������create�أ�
		// ��֣������������ȷʵҲ����˵�Ƕ�̬����
		m_Window = std::unique_ptr<Window>(Window::Create());
	}
	Application::~Application() {

	}
	void Application::Run() {
		while (m_Running) {
			m_Window->OnUpdate();

		}
	}
}