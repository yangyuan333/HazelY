#pragma once
#include "VertexArray.h"

namespace Hazel {
	/*
	* ���ݲ�ͬ��API���о����ʵ��
	* �˴�û��ʹ������ʱ��̬�����麯���ĽǶȳ���
	* ���Ǵ�static�ĽǶȣ�����premakeʱ��ѡ���Ӧ���ļ����б��룬��ӵ�ѡ����API
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

	// render�ľ���APIʵ��
	class RendererAPI {
	public:
		// �������ĺ���
		static void Clear(float r, float g, float b, float a);
		static void DrawIndexed(VertexArray* vertexArray, bool depthTest); // ҪVAO����VAOҲ����
		
		static void Init();
		static void Shutdown();
		static RenderAPICapabilities& GetCapabilities() {
			static RenderAPICapabilities capabilities;
			return capabilities;
		}

	public:
		// ��������
		static RendererAPIType GetAPI() { return s_CurrentRendererAPI; }
	private:
		static RendererAPIType s_CurrentRendererAPI;
	};

}