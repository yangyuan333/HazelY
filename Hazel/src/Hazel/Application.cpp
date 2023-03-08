#include "Application.h"
#include "Log.h"
#include <glad/glad.h>
#include "Hazel/Input.h"

namespace Hazel {
	
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		// ������һ�����⣬��������Ļ���ô�������create�أ�
		// ��֣������������ȷʵҲ����˵�Ƕ�̬����
		// �������Ҫ��̬��premake��windows��ֻ����windows���빹������������
		HZ_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlayer(m_ImGuiLayer);
	}
	
	Application::~Application() {

	}
	
	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		
		// HZ_CORE_TRACE("{0}", e.ToString());
		// ImGui�����һ��
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

	void Application::Run() {
		// Ϊʲô���ʱ��ImGui���¼�ϵͳû�б����ã�ImGui����һ�����弴��ϵͳ��
		// ���ˣ�GLFW����ϵͳ�ص��ĸ�Դ��Ŀǰֻ����imgui��opengl���֣�ֻ�ǻ��Ʋ��֣���򵥵ľ����ڴ˴�˳�����glfw��init
		while (m_Running) {

			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	
	void Application::PushOverlayer(Layer* layer) {
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}


}