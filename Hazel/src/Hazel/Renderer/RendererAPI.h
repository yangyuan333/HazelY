#pragma once
#include "VertexArray.h"

namespace Hazel {
	/*
	* 根据不同的API进行具体的实现
	* 此处没有使用运行时多态，从虚函数的角度出发
	* 而是从static的角度，根据premake时期选择对应的文件进行编译，间接的选择了API
	*/

	using RendererID = uint32_t;

	enum class HAZEL_API RendererAPIType
	{
		None,
		OpenGL
	};

	struct RenderAPICapabilities {
		std::string Vendor;
		std::string Renderer;
		std::string Version;

		int MaxSamples;
		float MaxAnisotropy;
	};

	// render的具体API实现
	class RendererAPI {
	public:
		// 两个核心函数
		static void Clear(float r, float g, float b, float a);
		static void DrawIndexed(VertexArray* vertexArray, bool depthTest); // 要VAO，把VAO也加上
		
		static void Init();
		static void Shutdown();
		static RenderAPICapabilities& GetCapabilities() {
			static RenderAPICapabilities capabilities;
			return capabilities;
		}

	public:
		// 辅助函数
		static RendererAPIType GetAPI() { return s_CurrentRendererAPI; }
	private:
		static RendererAPIType s_CurrentRendererAPI;
	};

}