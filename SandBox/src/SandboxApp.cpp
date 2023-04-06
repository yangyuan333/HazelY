#include <Hazel.h>
#include <Hazel/ImGui/ImGuiLayer.h>
#include <Hazel/Renderer/Renderer.h>
#include <Hazel/Renderer/Buffer.h>
#include <Hazel/Renderer/Shader.h>
#include <Hazel/Renderer/VertexArray.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <imgui.h>

class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer()
		: Layer("Example") {

	}
	// ��VAO���ĵ���������ڵ�command��ʽ
	// ��shader���ӿڻ���vs fs cs���ļ���ʽ
	// ��ѡ��İѱ����ڶ�̬�����������ڶ�̬---�Ȳ��ã�ûɶ̫��ı�Ҫ
	void OnAttach() override {
		/*
		*	1. ����VAO
		*	2. ������VBO
		*	3. ������EBO
		*	4. ��������Shader
		*/
		// ���VBO��EBO������������
		// ������static�Ĳ��У���ΪĿǰ��rendercommand���ӳٵģ���ֻ�ǽ�ָ�������ȥ�ˣ���û�а���ֵ����ȥ
		// ����bufferData�����������������ִ�е�
		// ̫����
		static float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};
		static unsigned int indices[] = {
			0, 1, 2
		};

		Hazel::BufferLayout m_layout = {
			{Hazel::ShaderDataType::Float3,"a_Position"}
		};
		std::shared_ptr<Hazel::VertexBuffer> m_vb{ Hazel::VertexBuffer::Create(vertices, sizeof(vertices)) };
		m_vb->SetLayout(m_layout);
		
		std::shared_ptr<Hazel::IndexBuffer> m_eb{ Hazel::IndexBuffer::Create(indices, sizeof(indices)/sizeof(unsigned int)) };

		m_vao = Hazel::VertexArray::Create();
		m_vao->AddVertexBuffer(m_vb);
		m_vao->SetIndexBuffer(m_eb);

		// Shader����
		m_shader = Hazel::Shader::Create("assets/shaders/shader.glsl");

	}
	void OnUpdate() override {
		// HZ_INFO("ExampleLayer::Update");
		m_shader->Bind();
		Hazel::Renderer::GetRenderer()->Clear(0.5, 0.5, 0.5, 1.0);

		m_vao->Bind();
		Hazel::Renderer::DrawIndexed(m_vao);
	}
	void OnEvent(Hazel::Event& event) override {
		// HZ_TRACE("{0}", event.ToString());
	}
	void OnImGuiRender() override
	{
		ImGui::Begin("Test1");
		ImGui::Text("Hello World");
		ImGui::End();
	}

private:
	Hazel::VertexArray* m_vao;
	//unsigned int m_vao;
	Hazel::Shader* m_shader;

};

class Sandbox: public Hazel::Application {
public:
	Sandbox() {
		
	}
	~Sandbox() override {

	}

	virtual void OnInit() override {
		PushLayer(new ExampleLayer());
		//PushOverlayer(new Hazel::ImGuiLayer());
	}
};

Hazel::Application* Hazel::CreateApplication() {
	return new Sandbox();
}