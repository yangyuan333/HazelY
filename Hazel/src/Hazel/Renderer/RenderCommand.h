//#pragma once
//
//#include "RendererAPI.h"
//#include <cstdint>
//
//namespace Hazel {
//	// 这个类后续也要继续进行优化，但是对于opengl来说，可能很难优化，因为opengl的指令是底层调用的，上层封装了，很难进行command级别的优化
//	// 但是对于渲染流程来说，还是可以进行优化的，例如相同材质的物体一起渲染，减少绑定切换的时间
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
//	* 这个类就是辅助CommandQueue进行实现的
//	* 主要作用就是提供renderer的同名函数，uint32_t(void*)，内部进行API调用实际处理
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
