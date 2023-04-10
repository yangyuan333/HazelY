#include <Hazel.h>
#include <Hazel/ImGui/ImGuiLayer.h>
#include <Hazel/Renderer/Renderer.h>
#include <Hazel/Renderer/Buffer.h>
#include <Hazel/Renderer/Shader.h>
#include <Hazel/Renderer/VertexArray.h>
#include <Hazel/Renderer/Texture.h>
#include <Hazel/Renderer/Framebuffer.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <imgui.h>

class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer()
		: Layer("Example") {

	}
	// 把VAO给改掉，结合现在的command形式
	// 改shader，接口换成vs fs cs的文件形式
	// 可选择的把编译期多态，换成运行期多态---先不用，没啥太大的必要
	void OnAttach() override {
		/*
		*	1. 创建VAO
		*	2. 创建绑定VBO
		*	3. 创建绑定EBO
		*	4. 创建生成Shader
		*/
		// 添加VBO和EBO的三角形例子
		// 必须是static的才行，因为目前的rendercommand是延迟的，他只是将指针给传进去了，而没有把数值传进去
		// 最终bufferData是在这个函数结束才执行的
		// 太蠢了
		static float vertices[] = {
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 -1.0f, 1.0f, 0.0f, 0.0f, 1.0f
		};
		static unsigned int indices[] = {
			0, 1, 2, 2, 3, 0
		};

		Hazel::BufferLayout m_layout = {
			{Hazel::ShaderDataType::Float3,"a_Position"},
			{Hazel::ShaderDataType::Float2, "a_Texcoord"}
		};
		std::shared_ptr<Hazel::VertexBuffer> m_vb{ Hazel::VertexBuffer::Create(vertices, sizeof(vertices)) };
		m_vb->SetLayout(m_layout);
		
		std::shared_ptr<Hazel::IndexBuffer> m_eb{ Hazel::IndexBuffer::Create(indices, sizeof(indices)/sizeof(unsigned int)) };

		m_vao = Hazel::VertexArray::Create();
		m_vao->AddVertexBuffer(m_vb);
		m_vao->SetIndexBuffer(m_eb);

		// Texture导入
		std::string texture_path = "assets/textures/environments/Arches_E_PineTree_Radiance.tga";
		m_texture = Hazel::TextureCubeMap::Create(texture_path, Hazel::TextureFormat::RGB, Hazel::TextureFormat::RGB, false);
		// Shader生成
		m_shader = Hazel::Shader::Create("assets/shaders/shader.glsl");
		// FrameBuffer生成
		m_framebuffer = Hazel::Framebuffer::Create({ 
			{Hazel::FramebufferTextureFormat::RGBA8,Hazel::FramebufferTextureFormat::RGBA8,Hazel::FramebufferTextureFormat::DEPTH24STENCIL8}
			});
	}
	void OnUpdate() override {
		// HZ_INFO("ExampleLayer::Update");
		m_shader->Bind();
		m_framebuffer->Bind();
		Hazel::Renderer::GetRenderer()->Clear(0.5, 0.5, 0.5, 1.0);
		
		Hazel::UniformBufferDeclaration
			<	sizeof(int) + sizeof(float)+
				sizeof(glm::vec2) + sizeof(glm::vec3) + sizeof(glm::vec4) + 
				sizeof(glm::mat3) + sizeof(glm::mat4)
			,	7> uniforms;
		uniforms.Push("a_int", 1);
		//uniforms.Push("a_unsigned", 2u);
		uniforms.Push("a_float", 100.0f);
		uniforms.Push("a_float2", glm::vec2(1, 2));
		uniforms.Push("a_float3", glm::vec3(1, 2, 3));
		uniforms.Push("a_float4", glm::vec4(1, 2, 3, 4));
		uniforms.Push("a_mat3", glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9));
		uniforms.Push("a_mat4", glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
		
		m_shader->UploadUniformBuffer(uniforms);

		m_texture->Bind(0);
		m_vao->Bind();
		
		Hazel::Renderer::DrawIndexed(m_vao, true);

		m_framebuffer->ShowFramebufferTexture(1);

		//HZ_RENDER_S(
		//	{
		//		glReadBuffer(GL_COLOR_ATTACHMENT1);
		//		glBlitNamedFramebuffer(
		//			self->m_framebuffer->GetRendererID(), 0,
		//			0, 0, self->m_framebuffer->GetWidth(), self->m_framebuffer->GetHeight(),
		//			0, 0, self->m_framebuffer->GetWidth(), self->m_framebuffer->GetHeight(),
		//			GL_COLOR_BUFFER_BIT, GL_LINEAR
		//		);
		//		self->m_framebuffer->Unbind();
		//	}
		//);

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
	Hazel::VertexArray* m_vao;
	//unsigned int m_vao;
	Hazel::Shader* m_shader;
	Hazel::TextureCubeMap* m_texture;
	Hazel::Framebuffer* m_framebuffer;

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