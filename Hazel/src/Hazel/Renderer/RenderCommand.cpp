//#include "RenderCommand.h"
////#include "Platform/OpenGL/OpenGLRendererAPI.h"
//
//namespace Hazel {
//
//	RendererAPI* RenderCommand::s_RendererAPI = new RendererAPI;
//
//	uint32_t RenderCommand::Clear(void* parameters) {
//		// 首先就是提取参数数量
//		float* data = (float*)parameters;
//		s_RendererAPI->Clear(data[0], data[1], data[2], data[3]);
//		return sizeof(float) * 4;
//	}
//
//}