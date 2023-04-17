#pragma once

#include "RenderCommandQueue.h"
#include "RendererCapabilities.h"
#include "RendererContext.h"
#include "RenderPass.h"
#include "Mesh.h"
#include "Material.h"
#include "SceneEnvironment.h"
#include "Image.h"
#include "Pipeline.h"
#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Hazel/Math/AABB.h"

#include "Hazel/Ref.h"

namespace Hazel {

	/*
	*	Renderer面向用户，属于顶层抽象封装
	*	用户指定“抽象功能”函数，Renderer内部将指令进行Command转换，并添加进CommandQueue中
	*	Command位函数指令级抽象，主要作用是转换参数，配合CommandQueue使用，内部配合RenderAPI进行具体的实现
	*/
	/*
	*	这个类不需要知道API的类别信息
	*/

	struct RendererConfig {
		// "Experimental" features
		bool ComputeEnvironmentMaps = false;
		// Tiering settings
		uint32_t EnvironmentMapResolution = 1024;
		uint32_t IrradianceMapComputeSamples = 512;
	};

	class Renderer {
	public:

		template <class FuncT>
		static void Submit(FuncT&& func) { 
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();
				pFunc->~FuncT();
			};
			auto storageBuffer = GetRenderCommandQueue().Allocate(renderCmd, sizeof(FuncT));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}
		static void WaitAndRender();

		static void Init();
		static void Shutdown();

		static void BeginFrame();
		static void EndFrame();
		static void BeginRenderPass(Ref<RenderPass> renderPass, bool clear = true);
		static void EndRenderPass();

		static void RenderMesh(Ref<Pipeline> pipeline, Ref<Mesh> mesh, const glm::mat4& transform);
		static void RenderMeshWithoutMaterial(Ref<Pipeline> pipeline, Ref<Mesh> mesh, const glm::mat4& transform);
		static void RenderQuad(Ref<Pipeline> pipeline, Ref<Material> material, const glm::mat4& transform);
		static void SubmitFullscreenQuad(Ref<Pipeline> pipeline, Ref<Material> material);

		static void DrawAABB(Ref<Mesh> mesh, const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawAABB(const AABB& aabb, const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));

		static void RegisterShaderDependency(Ref<Shader> shader, Ref<Pipeline> pipeline);
		static void RegisterShaderDependency(Ref<Shader> shader, Ref<Material> material);
		static void OnShaderReloaded(size_t hash);

		// Environment wait, Physical SkyLight Rendering
		static void SetSceneEnvironment(Ref<Environment> environment, Ref<Image2D> shadow);
		static std::pair<Ref<TextureCubeMap>, Ref<TextureCubeMap>> CreateEnvironmentMap(const std::string& filepath);
		static Ref<TextureCubeMap> CreatePreethamSky(float turbidity, float azimuth, float inclination);

		static Ref<RendererContext> GetContext();
		static RendererCapabilities& GetCapabilities();
		static Ref<ShaderLibrary> GetShaderLibrary();
		static Ref<Texture2D> GetWhiteTexture();
		static Ref<TextureCubeMap> GetBlackCubeTexture();
		static Ref<Environment> GetEmptyEnvironment();
		static RendererConfig& GetConfig();
	private:
		static RenderCommandQueue& GetRenderCommandQueue();
	};

}

/*
	class Renderer {
	public:
		typedef void(*RenderCommandFn)(void*);
		static void Init();
		// 核心函数
		static void Clear(float r, float g, float b, float a);
		static void DrawIndexed(VertexArray* vertexArray, bool depthTest);
		static void WaitAndRender();
	public:
		static void* Submit(RenderCommandFn fn, unsigned int size) {
			return s_Instance->m_CommandQueue.Allocate(fn, size);
		}
	public:
		inline static Renderer* GetRenderer() { return s_Instance; }
	private:
		static Renderer* s_Instance;
		RenderCommandQueue m_CommandQueue; // 这里为什么不设计成静态的呢？// 因为这个类本身是伪单例
	};

}

#define HZ_RENDER_PASTE2(a, b) a ## b
#define HZ_RENDER_PASTE(a, b) HZ_RENDER_PASTE2(a, b)
#define HZ_RENDER_UNIQUE(x) HZ_RENDER_PASTE(x, __LINE__)

#define HZ_RENDER(code) \
    struct HZ_RENDER_UNIQUE(HZRenderCommand) \
    {\
        static void Execute(void*)\
        {\
            code\
        }\
    };\
	{\
		auto mem = ::Hazel::Renderer::Submit(HZ_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(HZ_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) HZ_RENDER_UNIQUE(HZRenderCommand)();\
	}\

#define HZ_RENDER_1(arg0, code) \
	do {\
    struct HZ_RENDER_UNIQUE(HZRenderCommand) \
    {\
		HZ_RENDER_UNIQUE(HZRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0) \
		: arg0(arg0) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg0;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
    };\
	{\
		auto mem = ::Hazel::Renderer::Submit(HZ_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(HZ_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) HZ_RENDER_UNIQUE(HZRenderCommand)(arg0);\
	} } while(0)

#define HZ_RENDER_2(arg0, arg1, code) \
    struct HZ_RENDER_UNIQUE(HZRenderCommand) \
    {\
		HZ_RENDER_UNIQUE(HZRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1) \
		: arg0(arg0), arg1(arg1) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg1;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
    };\
	{\
		auto mem = ::Hazel::Renderer::Submit(HZ_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(HZ_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) HZ_RENDER_UNIQUE(HZRenderCommand)(arg0, arg1);\
	}\

#define HZ_RENDER_3(arg0, arg1, arg2, code) \
    struct HZ_RENDER_UNIQUE(HZRenderCommand) \
    {\
		HZ_RENDER_UNIQUE(HZRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2) \
		: arg0(arg0), arg1(arg1), arg2(arg2) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg1;\
			auto& arg2 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg2;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
    };\
	{\
		auto mem = ::Hazel::Renderer::Submit(HZ_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(HZ_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) HZ_RENDER_UNIQUE(HZRenderCommand)(arg0, arg1, arg2);\
	}\

#define HZ_RENDER_4(arg0, arg1, arg2, arg3, code) \
    struct HZ_RENDER_UNIQUE(HZRenderCommand) \
    {\
		HZ_RENDER_UNIQUE(HZRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3)\
		: arg0(arg0), arg1(arg1), arg2(arg2), arg3(arg3) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg1;\
			auto& arg2 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg2;\
			auto& arg3 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg3;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3;\
    };\
	{\
		auto mem = Renderer::Submit(HZ_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(HZ_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) HZ_RENDER_UNIQUE(HZRenderCommand)(arg0, arg1, arg2, arg3);\
	}

#define HZ_RENDER_5(arg0, arg1, arg2, arg3, arg4, code) \
    struct HZ_RENDER_UNIQUE(HZRenderCommand) \
    {\
		HZ_RENDER_UNIQUE(HZRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg4)>::type>::type arg4)\
		: arg0(arg0), arg1(arg1), arg2(arg2), arg3(arg3), arg4(arg4) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg1;\
			auto& arg2 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg2;\
			auto& arg3 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg3;\
			auto& arg4 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg4;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg4)>::type>::type arg4;\
    };\
	{\
		auto mem = Renderer::Submit(HZ_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(HZ_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) HZ_RENDER_UNIQUE(HZRenderCommand)(arg0, arg1, arg2, arg3, arg4);\
	}

#define HZ_RENDER_S(code) auto self = this;\
	HZ_RENDER_1(self, code)

#define HZ_RENDER_S1(arg0, code) auto self = this;\
	HZ_RENDER_2(self, arg0, code)

#define HZ_RENDER_S2(arg0, arg1, code) auto self = this;\
	HZ_RENDER_3(self, arg0, arg1, code)

#define HZ_RENDER_S3(arg0, arg1, arg2, code) auto self = this;\
	HZ_RENDER_4(self, arg0, arg1, arg2, code)

#define HZ_RENDER_S4(arg0, arg1, arg2, arg3, code) auto self = this;\
	HZ_RENDER_5(self, arg0, arg1, arg2, arg3, code)
*/