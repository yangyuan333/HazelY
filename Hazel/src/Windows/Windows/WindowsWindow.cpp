#include "WindowsWindow.h"
#include "Hazel/Log.h"

namespace Hazel {

	// Ϊʲô���ŵ�WindowsWindow�����еģ�
	// һ��GLFW���Դ�����Ӧ���windows��ֻ��Ҫinit GLFWһ�Σ������������������windows---��Ҳ������Ƴ�class��static
	static bool s_GLFWInitialized = false;

	// ��������������������ô���أ�
	// ����˵���൱������Ĭ��ΪWindowsWinow�ĺ�����
	// �ԣ������������Ҫ��д�Ļ��������Լ�������һ��static
	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		// �˴����Ƕ�̬����
		// ���캯���еĵ��ö��Ǿ�̬�󶨵�
		Init(props);
	}

	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props) {
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		//HZ_CORE_INFO("Creating window {0} ({1} {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized) {
			int success = glfwInit();
			HZ_ASSERT(success, "Could not intialize GLFW!");
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		// ��������Ǹ�ʲô�ģ�
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	}

	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate() {
		// �ȴ����¼�
		glfwPollEvents();
		// ��������֡
		glfwSwapBuffers(m_Window);
	}

	// ��Щ������������ 
	// �ؼ�����˭ͬ����ɶ��˼��
	void WindowsWindow::SetVSync(bool enabled) {
		if (enabled) {
			// ÿ����Ⱦ��һ֡ǰ������ȴ�һ��ͬ��
			glfwSwapInterval(1);
		}
		else {
			// ÿ����Ⱦ��һ֡ǰ�����ȴ�
			glfwSwapInterval(0);
		}
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const {
		return m_Data.VSync;
	}

}