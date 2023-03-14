#pragma once
#include "VertexArray.h"

namespace Hazel {
	/*
	* 根据不同的API进行具体的实现
	* 此处没有使用运行时多态，从虚函数的角度出发
	* 而是从static的角度，根据premake时期选择对应的文件进行编译，间接的选择了API
	*/

	enum class HAZEL_API RendererAPIType
	{
		None,
		OpenGL
	};

	// render的具体API实现
	class RendererAPI {
	public:
		// 两个核心函数
		static void Clear(float r, float g, float b, float a);
		static void DrawIndexed(VertexArray* vertexArray); // 要VAO，把VAO也加上
		
	public:
		// 辅助函数
		static RendererAPIType GetAPI() { return s_CurrentRendererAPI; }
	private:
		static RendererAPIType s_CurrentRendererAPI;
	};

}