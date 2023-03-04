#include "WindowsWindow.h"
#include "Hazel/Log.h"

namespace Hazel {

	// 为什么不放到WindowsWindow的类中的？
	// 一个GLFW可以创建对应多个windows，只需要init GLFW一次，所以这个变量不属于windows---但也可以设计成class的static
	static bool s_GLFWInitialized = false;

	// 那如果有其他的子类该怎么办呢？
	// 还是说这相当于是它默认为WindowsWinow的函数？
	// 对，其他子类如果要重写的话，必须自己再声明一个static
	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		// 此处不是动态调用
		// 构造函数中的调用都是静态绑定的
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
		// 这个函数是干什么的？
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	}

	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate() {
		// 先处理事件
		glfwPollEvents();
		// 交换缓存帧
		glfwSwapBuffers(m_Window);
	}

	// 这些都是声明东西 
	// 关键是与谁同步，啥意思？
	void WindowsWindow::SetVSync(bool enabled) {
		if (enabled) {
			// 每次渲染下一帧前，都会等待一次同步
			glfwSwapInterval(1);
		}
		else {
			// 每次渲染下一帧前，不等待
			glfwSwapInterval(0);
		}
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const {
		return m_Data.VSync;
	}

}