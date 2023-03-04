#include "Application.h"

namespace Hazel {
	Application::Application() {
		// 这里有一个问题，其他子类的话怎么调用这个create呢？
		// 奇怪，这个东西好像确实也不能说是多态啊？
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