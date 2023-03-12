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
	*	Renderer面向用户，属于顶层抽象封装
	*	用户指定“抽象功能”函数，Renderer内部将指令进行Command转换，并添加进CommandQueue中
	*	Command位函数指令级抽象，主要作用是转换参数，配合CommandQueue使用，内部配合RenderAPI进行具体的实现
	*/
	/*
	*	首先，以clear函数为例进行实现
	*/
	class Renderer {
	public:
		static void Clear(float r, float g, float b, float a);

		static Renderer* GetRenderer() { return s_Instance; }

		static void WaitAndRender();

	private:
		static Renderer* s_Instance;
		RenderCommandQueue m_CommandQueue; // 这里为什么不设计成静态的呢？// 因为这个类本身是伪单例
	};

}