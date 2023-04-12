#include <Hazel.h>
#include <Hazel/ImGui/ImGuiLayer.h>
#include <Hazel/Renderer/Renderer.h>
#include <Hazel/Renderer/Buffer.h>
#include <Hazel/Renderer/Shader.h>
#include <Hazel/Renderer/VertexArray.h>
#include <Hazel/Renderer/Texture.h>
#include <Hazel/Renderer/Framebuffer.h>
#include <Hazel/Renderer/Camera.h>
#include <Hazel/Editor/EditorCamera.h>
#include <Hazel/Timer.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

std::ostream& operator<<(std::ostream& os, glm::mat4 const& mat) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			os << mat[j][i] << " ";
		}
		os << std::endl;
	}
	return os;
}

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
		//static float vertices[] = {
		//	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		//	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		//	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		//	 -1.0f, 1.0f, 0.0f, 0.0f, 1.0f
		//};
		//static unsigned int indices[] = {
		//	0, 1, 2, 2, 3, 0
		//};

		/*
		* Create the Camera
		*/
		m_Camera = new Hazel::EditorCamera(
			glm::perspectiveFov(glm::radians(45.0f), 1280.0f, 720.0f, 0.1f, 10000.0f));

		static float vertices[8 * 3] = {
			1.000000,	-1.000000,	-1.000000,
			1.000000,	1.000000,	-1.000000,
			1.000000,	-1.000000,	1.000000 ,
			1.000000,	1.000000,	1.000000 ,
			-1.000000,	-1.000000,	-1.000000,
			-1.000000,	1.000000,	-1.000000,
			-1.000000,	-1.000000,	1.000000 ,
			-1.000000,	1.000000,	1.000000
		};

		static unsigned int indices[12*3] = {
			0,1,2,
			1,3,2,
			1,5,3,
			5,7,3,
			2,3,6,
			3,7,6,
			5,4,7,
			4,6,7,
			4,0,6,
			0,2,6,
			4,5,0,
			5,1,0
		};

		Hazel::BufferLayout m_layout = {
			{Hazel::ShaderDataType::Float3,"a_Position"}//,
			//{Hazel::ShaderDataType::Float2, "a_Texcoord"}
		};
		std::shared_ptr<Hazel::VertexBuffer> m_vb{ Hazel::VertexBuffer::Create(vertices, sizeof(vertices)) };
		m_vb->SetLayout(m_layout);
		
		std::shared_ptr<Hazel::IndexBuffer> m_eb{ Hazel::IndexBuffer::Create(indices, sizeof(indices)/sizeof(unsigned int)) };

		m_vao = Hazel::VertexArray::Create();
		m_vao->AddVertexBuffer(m_vb);
		m_vao->SetIndexBuffer(m_eb);

		// Texture导入
		//std::string texture_path = "assets/textures/environments/Arches_E_PineTree_Radiance.tga";
		//m_texture = Hazel::TextureCubeMap::Create(texture_path, Hazel::TextureFormat::RGB, Hazel::TextureFormat::RGB, false);
		// Shader生成
		m_shader = Hazel::Shader::Create("assets/shaders/shaderCamera.glsl");
		// FrameBuffer生成
		//m_framebuffer = Hazel::Framebuffer::Create({ 
		//	{Hazel::FramebufferTextureFormat::RGBA8,Hazel::FramebufferTextureFormat::RGBA8,Hazel::FramebufferTextureFormat::DEPTH24STENCIL8}
		//	});
	}
	void OnUpdate() override {

		Hazel::Timer timer;

		glm::mat4 mat_vp = m_Camera->GetViewProjection();

		glm::mat4 mat_v = m_Camera->GetViewMatrix();



		glm::mat4 mat_p = m_Camera->GetProjectionMatrix();

		std::cout << "ViewProjection:" << std::endl << mat_vp << std::endl;
		std::cout << "View:" << std::endl << mat_v << std::endl;
		std::cout << "Projection:" << std::endl << mat_p << std::endl;
		std::cout << "ViewProjection:" << std::endl << mat_p*mat_v << std::endl;

		// HZ_INFO("ExampleLayer::Update");
		m_shader->Bind();
		//m_framebuffer->Bind();
		Hazel::Renderer::GetRenderer()->Clear(0.5, 0.5, 0.5, 1.0);
		
		Hazel::UniformBufferDeclaration<
			sizeof(glm::mat4) + sizeof(glm::mat4), 2
		> uniforms;

		glm::mat4 modelmat;
		modelmat =
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05)) *
			glm::rotate(glm::mat4(1.0f), (float)0.0f / 4.0f, glm::vec3(1, 0, 0));
		uniforms.Push("ViewProjectionMatrix", mat_vp);
		uniforms.Push("ModelMatrix", modelmat);
		m_shader->UploadUniformBuffer(uniforms);

		// 

		//Hazel::UniformBufferDeclaration
		//	<	sizeof(int) + sizeof(float)+
		//		sizeof(glm::vec2) + sizeof(glm::vec3) + sizeof(glm::vec4) + 
		//		sizeof(glm::mat3) + sizeof(glm::mat4)
		//	,	7> uniforms;
		//uniforms.Push("a_int", 1);
		////uniforms.Push("a_unsigned", 2u);
		//uniforms.Push("a_float", 100.0f);
		//uniforms.Push("a_float2", glm::vec2(1, 2));
		//uniforms.Push("a_float3", glm::vec3(1, 2, 3));
		//uniforms.Push("a_float4", glm::vec4(1, 2, 3, 4));
		//uniforms.Push("a_mat3", glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9));
		//uniforms.Push("a_mat4", glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
		
		//m_shader->UploadUniformBuffer(uniforms);

		//m_texture->Bind(0);
		m_vao->Bind();
		
		Hazel::Renderer::DrawIndexed(m_vao, true);

		m_Camera->OnUpdate(Hazel::Timer::GetTime(timer, Hazel::Timer()));

		//m_framebuffer->ShowFramebufferTexture(1);

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
	Hazel::EditorCamera* m_Camera;

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