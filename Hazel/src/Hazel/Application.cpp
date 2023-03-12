#include "Application.h"
#include "Log.h"
#include <glad/glad.h>
#include "Hazel/Input.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
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

		//m_ImGuiLayer = new ImGuiLayer();
		//PushOverlayer(m_ImGuiLayer);

		// ��ϰһ��opengl�Ļ������̺͸�����
		m_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f,-0.5f,0.0f, 1.0f, 0.0f ,1.0f,1.0f,
			0.5f,-0.5f,0.0f, 0.0f, 1.0f ,1.0f,1.0f,
			0.0f,0.5f,0.0f, 1.0f, 1.0f ,0.0f,1.0f
		};
		unsigned int index[3] = {
			0,1,2
		};

		std::shared_ptr<VertexBuffer> m_VertexBuffer(VertexBuffer::Create(vertices, sizeof(vertices)));
		m_VertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"}
			});
		
		std::shared_ptr<IndexBuffer> m_IndexBuffer(IndexBuffer::Create(index, sizeof(index) / sizeof(uint32_t)));
		
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			out vec3 v_Position;
			void main()
			{
				gl_Position = vec4(a_Position,1.0f);
				//v_Position = (a_Position + 1.0f)/2.0f;
				v_Position = a_Color.xyz;
			}
	
		)";	

		std::string pragmentSrc = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			in vec3 v_Position;

			void main()
			{
				//color = vec4(0.8,0.2,0.3,1.0);
				color = vec4(v_Position,1.0f);
			}
		)";

		m_Shader = std::make_shared<Shader>(vertexSrc, pragmentSrc);
		m_Shader->Bind();
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
		OnInit();
		while (m_Running) {

			RenderCommand::SetClearColor(glm::vec4{ 0.5f, 0.5f, 0.5f ,1.0f });
			RenderCommand::Clear();
			Renderer::BeginScene();
			Renderer::Submit(m_VertexArray);
			Renderer::EndScene();

			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			ImGuiLayer::Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
			ImGuiLayer::End();

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