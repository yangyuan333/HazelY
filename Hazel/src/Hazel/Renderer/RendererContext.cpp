#include "RendererContext.h"
#include "Platform/OpenGL/OpenglContext.h"
#include "RendererAPI.h"

namespace Hazel {

	Ref<RendererContext> RendererContext::Create(GLFWwindow* windowHandle) {
		switch (RendererAPI::GetAPI())
		{
		case RendererAPIType::OpenGL: return Ref<OpenGLContext>::Create(windowHandle);
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}