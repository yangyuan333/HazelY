#include "VertexArray.h"
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Hazel{

	Ref<VertexArray> VertexArray::Create() {
		switch (RendererAPI::Current())
		{
		case(RendererAPIType::OpenGL): return Ref<OpenGLVertexArray>::Create();
		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}