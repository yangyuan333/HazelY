#include "WindowsWindow.h"
#include "Hazel/Log.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Platform/OpenGL/OpenglContext.h"
#include <glad/glad.h>
#include <imgui.h>

namespace Hazel {

	// Ϊʲô���ŵ�WindowsWindow�����еģ�
	// һ��GLFW���Դ�����Ӧ���windows��ֻ��Ҫinit GLFWһ�Σ������������������windows---��Ҳ������Ƴ�class��static
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		HZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	// ��������������������ô���أ�
	// ����˵���൱������Ĭ��ΪWindowsWinow�ĺ�����
	// �ԣ������������Ҫ��д�Ļ��������Լ�������һ��static
	// ����...������Ƕ�̬����Ϊ��ͬ��ƽ̨�в�ͬ��ʵ�֣������linux����windows���ļ������룬���Բ��ᱨ��
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
			glfwSetErrorCallback(GLFWErrorCallback);
			int success = glfwInit();
			HZ_ASSERT(success, "Could not intialize GLFW!");
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		
		// m_RendererContext = new OpenGLContext(m_Window);
		m_RendererContext = RendererContext::Create(m_Window);
		m_RendererContext->Create();

		// ��������Ǹ�ʲô�ģ�
		glfwSetWindowUserPointer(m_Window, &m_Data);
		
		// ��ֱͬ��
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
				}
			});

		// ��һ�������־�¼���������ʲô
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) 
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyTypedEvent keyTypedEvent(keycode);
			
				data.EventCallback(keyTypedEvent);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});

		// ������ͬ�������״
		m_ImGuiMouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		m_ImGuiMouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
		m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);   // FIXME: GLFW doesn't have this.
		m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
		m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
		m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
		m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
		m_ImGuiMouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

		// ��̫��Ҫ
		// Update window size to actual size
		{
			int width, height;
			glfwGetWindowSize(m_Window, &width, &height);
			m_Data.Width = width;
			m_Data.Height = height;
		}
	}

	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(m_Window); // �����ȽϺã���Ϊ��Ҫ���ǵ��ര��ϵͳ
	}

	std::pair<float, float> WindowsWindow::GetWindowPos() const
	{
		int x, y;
		glfwGetWindowPos(m_Window, &x, &y);
		return { x, y };
	}

	void WindowsWindow::ProcessEvents()
	{
		glfwPollEvents();

		ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
		glfwSetCursor(m_Window, m_ImGuiMouseCursors[imgui_cursor] ? m_ImGuiMouseCursors[imgui_cursor] : m_ImGuiMouseCursors[ImGuiMouseCursor_Arrow]);
		//glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void WindowsWindow::SwapBuffers()
	{
		m_RendererContext->SwapBuffers();
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

	void WindowsWindow::Maximize()
	{
		glfwMaximizeWindow(m_Window);
	}

	void WindowsWindow::SetTitle(const std::string& title)
	{
		m_Data.Title = title;
		glfwSetWindowTitle(m_Window, m_Data.Title.c_str());
	}

}