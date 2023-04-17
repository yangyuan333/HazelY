#pragma once
#include "Renderer.h"
#include "RendererCapabilities.h"
#include "VertexArray.h"

namespace Hazel {
	/*
	* 根据不同的API进行具体的实现
	* 此处没有使用运行时多态，从虚函数的角度出发
	* 而是从static的角度，根据premake时期选择对应的文件进行编译，间接的选择了API
	*/

	using RendererID = uint32_t;

	enum class RendererAPIType
	{
		None,
		OpenGL
	};

	enum class PrimitiveType
	{
		None = 0, Triangles, Lines
	};

	class RendererAPI {
	public:
		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void BeginRenderPass(const Ref<RenderPass>& renderPass) = 0;
		virtual void EndRenderPass() = 0;

		virtual void RenderMesh(Ref<Pipeline> pipeline, Ref<Mesh> mesh, const glm::mat4& transform) = 0;
		virtual void RenderMeshWithoutMaterial(Ref<Pipeline> pipeline, Ref<Mesh> mesh, const glm::mat4& transform) = 0;
		virtual void RenderQuad(Ref<Pipeline> pipeline, Ref<Material> material, const glm::mat4& transform) = 0;
		virtual void SubmitFullscreenQuad(Ref<Pipeline> pipeline, Ref<Material> material) = 0;

		virtual void SetSceneEnvironment(Ref<Environment> environment, Ref<Image2D> shadow) = 0;
		virtual std::pair<Ref<TextureCubeMap>, Ref<TextureCubeMap>> CreateEnvironmentMap(const std::string& filepath) = 0;
		virtual Ref<TextureCubeMap> CreatePreethamSky(float turbidity, float azimuth, float inclination) = 0;

		virtual RendererCapabilities& GetCapabilities() = 0;
		static RendererAPIType Current() { return s_CurrentRendererAPI; }
		static void SetAPI(RendererAPIType api);
	private:
		inline static RendererAPIType s_CurrentRendererAPI = RendererAPIType::OpenGL;
	};
}
	/*
	// render的具体API实现
	class RendererAPI {
	public:
		// 两个核心函数
		static void Clear(float r, float g, float b, float a);
		static void DrawIndexed(VertexArray* vertexArray, bool depthTest); // 要VAO，把VAO也加上

		static void Init();
		static void Shutdown();
		static RendererCapabilities& GetCapabilities() {
			static RendererCapabilities capabilities;
			return capabilities;
		}

	public:
		// 辅助函数
		static RendererAPIType GetAPI() { return s_CurrentRendererAPI; }
	private:
		static RendererAPIType s_CurrentRendererAPI;
	};
	*/