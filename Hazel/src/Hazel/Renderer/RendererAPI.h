#pragma once

namespace Hazel {

	/*
	class RendererAPI {
	public:
		enum class API {
			None = 0,
			OpenGL = 1
		};
	public:
		virtual void SetClearColor(glm::vec4 const& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
	*/

	/*
	* 根据不同的API进行具体的实现
	* 此处没有使用运行时多态，从虚函数的角度出发
	* 而是从static的角度，根据premake时期选择对应的文件进行编译，间接的选择了API
	*/
	class RendererAPI {
	public:
		static void Clear(float r, float g, float b, float a);
	};

}