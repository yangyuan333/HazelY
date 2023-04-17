#pragma once
#include "Renderer.h"
#include "RendererCapabilities.h"
#include "VertexArray.h"

namespace Hazel {
	/*
	* ���ݲ�ͬ��API���о����ʵ��
	* �˴�û��ʹ������ʱ��̬�����麯���ĽǶȳ���
	* ���Ǵ�static�ĽǶȣ�����premakeʱ��ѡ���Ӧ���ļ����б��룬��ӵ�ѡ����API
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
	// render�ľ���APIʵ��
	class RendererAPI {
	public:
		// �������ĺ���
		static void Clear(float r, float g, float b, float a);
		static void DrawIndexed(VertexArray* vertexArray, bool depthTest); // ҪVAO����VAOҲ����

		static void Init();
		static void Shutdown();
		static RendererCapabilities& GetCapabilities() {
			static RendererCapabilities capabilities;
			return capabilities;
		}

	public:
		// ��������
		static RendererAPIType GetAPI() { return s_CurrentRendererAPI; }
	private:
		static RendererAPIType s_CurrentRendererAPI;
	};
	*/