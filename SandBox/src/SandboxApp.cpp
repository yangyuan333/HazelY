#include <Hazel.h>
#include <Hazel/ImGui/ImGuiLayer.h>
#include <Hazel/Renderer/VertexArray.h>
#include <Hazel/Renderer/Shader.h>
#include <glm/glm.hpp>

#include <imgui.h>

#include <Hazel/Renderer/Renderer.h>

class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer()
		: Layer("Example") {

	}

	void OnAttach() override {
		// 添加渲染对象
		// 画一个三角形看看
		m_VertexArray.reset(Hazel::VertexArray::Create());
		// 添加VBO
		float vertex[3 * 3] = {
			-0.5f,-0.5f,0.0f,
			0.5f,-0.5f,0.0f,
			0.0f,0.5f,0.0f
		};
			
		std::shared_ptr<Hazel::VertexBuffer> vertexBuffer(Hazel::VertexBuffer::Create(vertex, sizeof(vertex)));
		vertexBuffer->SetLayout(
			{
				{Hazel::ShaderDataType::Float3,"a_Position"}
			}
		);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int index[3] = { 0,1,2 };
		std::shared_ptr<Hazel::IndexBuffer> indexBuffer(Hazel::IndexBuffer::Create(index, sizeof(index)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		// 创建shader
		std::string vertexSrc = R"(
			#version 430

			layout(location = 0) in vec3 a_Position;

			void main()
			{
				gl_Position = vec4(a_Position, 1.0);
			}
	
		)";

		std::string pragmentSrc = R"(
			#version 430

			layout(location = 0) out vec4 finalColor;

			//uniform vec4 u_Color;

			void main()
			{
				finalColor = vec4(1.0f,0.0f,0.0f,1.0f);
			}
		)";

		m_Shader.reset(new Hazel::Shader(vertexSrc, pragmentSrc));
		// m_Shader->Bind();
	
	}

	void OnUpdate() override {

		Hazel::Renderer::GetRenderer()->Clear(0.5, 0.5, 0.5, 1.0);

		m_Shader->Bind();
		m_VertexArray->Bind();

		Hazel::Renderer::DrawIndexed(m_VertexArray.get());

	}
	void OnEvent(Hazel::Event& event) override {
		
	}
	void OnImGuiRender() override
	{
		ImGui::Begin("Test1");
		ImGui::Text("Hello World");
		ImGui::End();
	}

private:
	std::unique_ptr<Hazel::VertexArray> m_VertexArray;
	std::unique_ptr<Hazel::Shader> m_Shader;

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