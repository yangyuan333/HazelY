#include "Image.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLImage.h"

namespace Hazel {
	Ref<Image2D> Image2D::Create(ImageFormat format, uint32_t width, uint32_t height, Buffer buffer)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::OpenGL: Ref<OpenGLImage2D>::Create(format, width, height, buffer);
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
	Ref<Image2D> Image2D::Create(ImageFormat format, uint32_t width, uint32_t height, const void* data)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::OpenGL: Ref<OpenGLImage2D>::Create(format, width, height, data);
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}