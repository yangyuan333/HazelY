#include "Application.h"
#include "Log.h"
#include <glad/glad.h>
#include "Hazel/Input.h"
//#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Hazel/Renderer/Renderer.h"
// #include "Hazel/Renderer/RenderCommand.h"

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

		m_ImGuiLayer.reset(new ImGuiLayer());
		PushOverlayer(m_ImGuiLayer.get()); // ���������ImGui�ĳ�ʼ��

		Renderer::Init();
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

	void Application::RenderImGui() {
		ImGuiLayer::Begin();
		for (Layer* layer : m_LayerStack) {
			layer->OnImGuiRender();
		}
		ImGuiLayer::End();
	}

	void Application::Run() {
		// Ϊʲô���ʱ��ImGui���¼�ϵͳû�б����ã�ImGui����һ�����弴��ϵͳ��
		// ���ˣ�GLFW����ϵͳ�ص��ĸ�Դ��Ŀǰֻ����imgui��opengl���֣�ֻ�ǻ��Ʋ��֣���򵥵ľ����ڴ˴�˳�����glfw��init
		OnInit();
		while (m_Running) {
			/*
			RenderCommand::SetClearColor(glm::vec4{ 0.5f, 0.5f, 0.5f ,1.0f });
			RenderCommand::Clear();
			Renderer::BeginScene();
			Renderer::Submit(m_VertexArray);
			Renderer::EndScene();
			*/

			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			Application* app = this;
			HZ_RENDER_1(
				app,
				{ 
					app->RenderImGui();
				}
			);

			Renderer::GetRenderer()->WaitAndRender();
			m_Window->OnUpdate();
		}
		OnShutdown();
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