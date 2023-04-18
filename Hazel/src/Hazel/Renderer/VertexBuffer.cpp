#include "VertexBuffer.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Hazel {

	Ref<VertexBuffer> VertexBuffer::Create(void* vertices, uint32_t size, VertexBufferUsage usage)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::OpenGL: Ref<OpenGLVertexBuffer>::Create(vertices, size, usage);
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, VertexBufferUsage usage)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::OpenGL: Ref<OpenGLVertexBuffer>::Create(size, usage);
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}