#pragma once

#include "VertexArray.h"

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
	* �����ȱ�ĳɶ�̬���ԣ��Ȳ���������ʱ�л�API������
	*/

	class RendererAPI {
	public:
		enum class API {
			None = 0,
			OpenGL = 1
		};
	public:
		static void Clear(float r, float g, float b, float a = 1.0f);
		static void DrawIndexed(VertexArray* vertexArray);

		inline static API GetAPI() { return s_API; }
		//static RendererAPI* Create();
	private:
		static API s_API;
	};

}