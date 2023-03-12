#pragma once

#include "RenderCommandQueue.h"

namespace Hazel {

	//class Renderer {
	//public:
	//	static void BeginScene(); // TODO
	//	static void EndScene(); // TODO
	//	static void Submit(const std::shared_ptr<VertexArray>& vertexArray); // TODO
	//	
	//public:
	//	inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	//};

	/*
	*	Renderer�����û������ڶ�������װ
	*	�û�ָ���������ܡ�������Renderer�ڲ���ָ�����Commandת��������ӽ�CommandQueue��
	*	Commandλ����ָ�������Ҫ������ת�����������CommandQueueʹ�ã��ڲ����RenderAPI���о����ʵ��
	*/
	/*
	*	���ȣ���clear����Ϊ������ʵ��
	*/
	class Renderer {
	public:
		static void Clear(float r, float g, float b, float a);

		static Renderer* GetRenderer() { return s_Instance; }

		static void WaitAndRender();

	private:
		static Renderer* s_Instance;
		RenderCommandQueue m_CommandQueue; // ����Ϊʲô����Ƴɾ�̬���أ�// ��Ϊ����౾����α����
	};

}