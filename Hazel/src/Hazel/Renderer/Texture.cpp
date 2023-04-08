#include "Texture.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Hazel {
	Texture2D* Texture2D::Create(std::string const& path, TextureFormat innerFormat, TextureFormat outerFormat, bool srgb) {
		
		switch (RendererAPI::GetAPI())
		{
		case RendererAPIType::OpenGL:
			return new OpenGLTexture2D(path, innerFormat, outerFormat, srgb);
		default:
			return nullptr;
		}
		int a;
		a = 1;
	}

	Texture2D* Texture2D::Create(TextureFormat format, unsigned int width, unsigned int height, bool srgb) {
		
		switch (RendererAPI::GetAPI())
		{
		case RendererAPIType::OpenGL:
			return new OpenGLTexture2D(format, width, height, srgb);
		default:
			return nullptr;
		}

	}

	TextureCubeMap* TextureCubeMap::Create(std::string const& path, TextureFormat innerFormat, TextureFormat outerFormat, bool srgb) {
		
		switch (RendererAPI::GetAPI())
		{
		case RendererAPIType::OpenGL:
			return new OpenGLTextureCubeMap(path, innerFormat, outerFormat, srgb);
		default:
			return nullptr;
		}

	}

	TextureCubeMap* TextureCubeMap::Create(TextureFormat format, unsigned int width, unsigned int height, bool srgb) {
		
		switch (RendererAPI::GetAPI())
		{
		case RendererAPIType::OpenGL:
			return new OpenGLTextureCubeMap(format, width, height, srgb);
		default:
			return nullptr;
		}

	}

}