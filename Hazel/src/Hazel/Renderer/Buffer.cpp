#include "Buffer.h"
#include "Renderer.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Hazel {
	
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (RendererAPI::GetAPI())
		{
		case(RendererAPIType::None):
		{
			HZ_CORE_ASSERT(false, "RendererAPI::None, is currently not supported!");
			return nullptr;
		}
		case(RendererAPIType::OpenGL):
		{
			return new OpenGLVertexBuffer(vertices, size);
		}
		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) {
		switch (RendererAPI::GetAPI())
		{
		case(RendererAPIType::None):
		{
			HZ_CORE_ASSERT(false, "RendererAPI::None, is currently not supported!");
			return nullptr;
		}
		case(RendererAPIType::OpenGL):
		{
			return new OpenGLIndexBuffer(indices, size);
		}
		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}