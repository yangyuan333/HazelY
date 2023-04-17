#include "Renderer.h"
#include "RendererAPI.h"
#include "RenderCommandQueue.h"
#include "Platform/OpenGL/OpenGLRenderer.h"

namespace Hazel {

	/// <summary>
	/// Renderer Data List
	/// using the static vars, ensure vars only be used in renderer.
	/// </summary>
	struct RendererData
	{
		RendererConfig Config;
		Ref<ShaderLibrary> m_ShaderLibrary;
		Ref<Texture2D> WhiteTexture;
		Ref<TextureCubeMap> BlackCubeTexture;
		Ref<Environment> EmptyEnvironment;
	};
	struct ShaderDependencies
	{
		std::vector<Ref<Pipeline>> Pipelines;
		std::vector<Ref<Material>> Materials;
	};
	static RendererData* s_Data = nullptr;
	static std::unordered_map<size_t, ShaderDependencies> s_ShaderDependencies;
	static RendererAPI* s_RendererAPI = nullptr;
	static RenderCommandQueue* s_CommandQueue = nullptr;
	static std::unordered_map<size_t, Ref<Pipeline>> s_PipelineCache;

	static RendererAPI* InitRendererAPI()
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::OpenGL: return new OpenGLRenderer();
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	/// <summary>
	/// Renderer Function List
	/// </summary>
	void Renderer::WaitAndRender()
	{
		GetRenderCommandQueue().Execute();
	}

	void Renderer::Init()
	{
		/*
		* 1. Init the RendererAPI, RendererCommandQueue, RendererData
		* 2. Import the Shader Library, eg. load the shaders before using.
		* 3. 
		*/
		s_Data = new RendererData();
		s_CommandQueue = new RenderCommandQueue();
		s_RendererAPI = InitRendererAPI();

		/*
		* Existing Shaders
		* Load the All Shaders and Compile the Shaders
		*/
		s_Data->m_ShaderLibrary = Ref<ShaderLibrary>::Create();
		// GetShaderLibrary()->Load("assets/shaders/shader.glsl");
		Renderer::WaitAndRender();

		/*
		* Init the RendererData
		*/
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture = Texture2D::Create(ImageFormat::RGBA, 1, 1, &whiteTextureData);

		uint32_t blackTextureData[6] = { 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000 };
		s_Data->BlackCubeTexture = TextureCube::Create(ImageFormat::RGBA, 1, 1, &blackTextureData);

		s_Data->EmptyEnvironment = Ref<Environment>::Create(s_Data->BlackCubeTexture, s_Data->BlackCubeTexture);

		s_RendererAPI->Init();
		SceneRenderer::Init();

	}

	void Renderer::Shutdown()
	{
	}

	void Renderer::BeginFrame()
	{
	}

	void Renderer::EndFrame()
	{
	}

	void Renderer::BeginRenderPass(Ref<RenderPass> renderPass, bool clear)
	{
	}

	void Renderer::EndRenderPass()
	{
	}

	void Renderer::RenderMesh(Ref<Pipeline> pipeline, Ref<Mesh> mesh, const glm::mat4& transform)
	{
	}

	void Renderer::RenderMeshWithoutMaterial(Ref<Pipeline> pipeline, Ref<Mesh> mesh, const glm::mat4& transform)
	{
	}

	void Renderer::RenderQuad(Ref<Pipeline> pipeline, Ref<Material> material, const glm::mat4& transform)
	{
	}

	void Renderer::SubmitFullscreenQuad(Ref<Pipeline> pipeline, Ref<Material> material)
	{
	}

	void Renderer::DrawAABB(Ref<Mesh> mesh, const glm::mat4& transform, const glm::vec4& color)
	{
	}

	void Renderer::DrawAABB(const AABB& aabb, const glm::mat4& transform, const glm::vec4& color)
	{
	}

	void Renderer::RegisterShaderDependency(Ref<Shader> shader, Ref<Pipeline> pipeline)
	{
	}

	void Renderer::OnShaderReloaded(size_t hash)
	{
	}

	void Renderer::SetSceneEnvironment(Ref<Environment> environment, Ref<Image2D> shadow)
	{
	}

	std::pair<Ref<TextureCubeMap>, Ref<TextureCubeMap>> Renderer::CreateEnvironmentMap(const std::string& filepath)
	{
		return std::pair<Ref<TextureCubeMap>, Ref<TextureCubeMap>>();
	}

	Ref<TextureCubeMap> Renderer::CreatePreethamSky(float turbidity, float azimuth, float inclination)
	{
		return Ref<TextureCubeMap>();
	}

	Ref<RendererContext> Renderer::GetContext()
	{
		return Ref<RendererContext>();
	}

	RendererCapabilities& Renderer::GetCapabilities()
	{
		// TODO: 在此处插入 return 语句
	}

	Ref<ShaderLibrary> Renderer::GetShaderLibrary()
	{
		return s_Data->m_ShaderLibrary;
	}

	Ref<Texture2D> Renderer::GetWhiteTexture()
	{
		return Ref<Texture2D>();
	}

	Ref<TextureCubeMap> Renderer::GetBlackCubeTexture()
	{
		return Ref<TextureCubeMap>();
	}

	Ref<Environment> Renderer::GetEmptyEnvironment()
	{
		return Ref<Environment>();
	}

	RendererConfig& Renderer::GetConfig()
	{
		// TODO: 在此处插入 return 语句
	}

	RenderCommandQueue& Renderer::GetRenderCommandQueue()
	{
		// TODO: 在此处插入 return 语句
	}



}