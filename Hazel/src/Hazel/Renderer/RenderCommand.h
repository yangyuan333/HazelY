//#pragma once
//
//#include "RendererAPI.h"
//#include <cstdint>
//
//namespace Hazel {
//	// ��������ҲҪ���������Ż������Ƕ���opengl��˵�����ܺ����Ż�����Ϊopengl��ָ���ǵײ���õģ��ϲ��װ�ˣ����ѽ���command������Ż�
//	// ���Ƕ�����Ⱦ������˵�����ǿ��Խ����Ż��ģ�������ͬ���ʵ�����һ����Ⱦ�����ٰ��л���ʱ��
//	//class RenderCommand {
//	//public:
//	//	inline static void SetClearColor(glm::vec4 const& color) {
//	//		s_RendererAPI->SetClearColor(color);
//	//	}
//	//	inline static void Clear() {
//	//		s_RendererAPI->Clear();
//	//	}
//	//	inline static void DrawIndexed(std::shared_ptr<VertexArray>const& vertexArray) {
//	//		s_RendererAPI->DrawIndexed(vertexArray);
//	//	}
//	//private:
//	//	static RendererAPI* s_RendererAPI;
//	//};
//
//	/*
//	* �������Ǹ���CommandQueue����ʵ�ֵ�
//	* ��Ҫ���þ����ṩrenderer��ͬ��������uint32_t(void*)���ڲ�����API����ʵ�ʴ���
//	*/
//
//	class RenderCommand {
//	public:
//		
//		static uint32_t Clear(void* parameters);
//
//	private:
//		static RendererAPI* s_RendererAPI;
//	};
//
//}
//
