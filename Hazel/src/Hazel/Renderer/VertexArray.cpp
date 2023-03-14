#include "VertexArray.h"
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Hazel{

	VertexArray* VertexArray::Create() {
		switch (RendererAPI::GetAPI())
		{
		case(RendererAPIType::None):
		{
			HZ_CORE_ASSERT(false, "RendererAPI::None, is currently not supported!");
			return nullptr;
		}
		case(RendererAPIType::OpenGL):
		{
			return new OpenGLVertexArray();
		}
		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}