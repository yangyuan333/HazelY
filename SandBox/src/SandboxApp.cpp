#include <Hazel.h>
#include <Hazel/ImGui/ImGuiLayer.h>
#include <Hazel/Renderer/Renderer.h>
#include <Hazel/Renderer/Buffer.h>
#include <Hazel/Renderer/Shader.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <imgui.h>

class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer()
		: Layer("Example") {

	}
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
		Hazel::VertexBuffer* m_vb = Hazel::VertexBuffer::Create(vertices, sizeof(vertices)); // ע�⣺����Ҳ�ͽ����
		
		Hazel::IndexBuffer* m_eb = Hazel::IndexBuffer::Create(indices, sizeof(indices)); // ע�⣺����Ҳ�ͽ����
		

		HZ_RENDER_S(
			{
				glCreateVertexArrays(1,&(self->m_vao));
				glBindVertexArray(self->m_vao);
			}
		);

		m_vb->Bind();
		HZ_RENDER(
			{
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
			}
		);
		m_eb->Bind();

		HZ_RENDER(
			{
				glBindVertexArray(0);
			}
		);

		// Shader����
		m_shader = Hazel::Shader::Create("assets/shaders/shader.glsl");

	}
	void OnUpdate() override {
		// HZ_INFO("ExampleLayer::Update");
		m_shader->Bind();
		Hazel::Renderer::GetRenderer()->Clear(0.5, 0.5, 0.5, 1.0);

		HZ_RENDER_S(
			{
				glBindVertexArray(self->m_vao);
				glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
			}
		);
	}
	void OnEvent(Hazel::Event& event) override {
		// HZ_TRACE("{0}", event.ToString());
	}
	void OnImGuiRender() override
	{
		//ImGui::Begin("Test1");
		//ImGui::Text("Hello World");
		//ImGui::End();
	}

private:
	unsigned int m_vao;
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