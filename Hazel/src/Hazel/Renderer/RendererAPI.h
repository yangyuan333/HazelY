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
	* ���ݲ�ͬ��API���о����ʵ��
	* �˴�û��ʹ������ʱ��̬�����麯���ĽǶȳ���
	* ���Ǵ�static�ĽǶȣ�����premakeʱ��ѡ���Ӧ���ļ����б��룬��ӵ�ѡ����API
	*/
	class RendererAPI {
	public:
		static void Clear(float r, float g, float b, float a);
	};

}