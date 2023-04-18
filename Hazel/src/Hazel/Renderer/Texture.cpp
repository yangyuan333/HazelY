#include "Texture.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Hazel {
	Ref<Texture2D> Texture2D::Create(const std::string& path, TextureProperties properties) {
		
		switch (RendererAPI::Current())
		{
		case RendererAPIType::OpenGL:
			return Ref<OpenGLTexture2D>::Create(path, properties);
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(ImageFormat format, uint32_t width, uint32_t height, const void* data, TextureProperties properties) {
		
		switch (RendererAPI::Current())
		{
		case RendererAPIType::OpenGL:
			return Ref<OpenGLTexture2D>::Create(format, width, height, data, properties);
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<TextureCubeMap> TextureCubeMap::Create(const std::string& path, TextureProperties properties) {
		
		switch (RendererAPI::Current())
		{
		case RendererAPIType::OpenGL:
			return Ref<OpenGLTextureCubeMap>::Create(path, properties);
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<TextureCubeMap> TextureCubeMap::Create(ImageFormat format, uint32_t width, uint32_t height, const void* data, TextureProperties properties) {
		
		switch (RendererAPI::Current())
		{
		case RendererAPIType::OpenGL:
			return Ref<OpenGLTextureCubeMap>::Create(format, width, height, data, properties);
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}