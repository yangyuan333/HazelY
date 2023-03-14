#pragma once
#include "VertexArray.h"

namespace Hazel {
	/*
	* ���ݲ�ͬ��API���о����ʵ��
	* �˴�û��ʹ������ʱ��̬�����麯���ĽǶȳ���
	* ���Ǵ�static�ĽǶȣ�����premakeʱ��ѡ���Ӧ���ļ����б��룬��ӵ�ѡ����API
	*/

	enum class HAZEL_API RendererAPIType
	{
		None,
		OpenGL
	};

	// render�ľ���APIʵ��
	class RendererAPI {
	public:
		// �������ĺ���
		static void Clear(float r, float g, float b, float a);
		static void DrawIndexed(VertexArray* vertexArray); // ҪVAO����VAOҲ����
		
	public:
		// ��������
		static RendererAPIType GetAPI() { return s_CurrentRendererAPI; }
	private:
		static RendererAPIType s_CurrentRendererAPI;
	};

}