#pragma once
#include "Hazel/Renderer/Texture.h"
#include "glad/glad.h"

namespace Hazel {

	class OpenGLTexture2D :public Texture2D {
	public:
		virtual ~OpenGLTexture2D() {}
		inline RendererID GetRendererID() const override { return m_RedererId; }
		inline TextureFormat GetFormat() const override { return m_TextureFormat; }
		inline unsigned int GetWidth() const override { return m_Width; }
		inline unsigned int GetHeight() const override { return m_Height; }

		void Bind(unsigned int slot) override;
		
		OpenGLTexture2D(std::string const& path, bool srgb = true);
		OpenGLTexture2D(TextureFormat format, unsigned int width, unsigned int height, bool srgb = false);

		void UpdateTexture2D(char const* data, TextureFormat format = TextureFormat::RGB, unsigned int type = GL_UNSIGNED_BYTE, unsigned int x_offset = 0, unsigned int y_offset = 0, unsigned int width = 0, unsigned int height = 0);

	private:
		RendererID m_RedererId;
		TextureFormat m_TextureFormat;
		unsigned int m_Width;
		unsigned int m_Height;
	};

	class OpenGLTextureCubeMap :public TextureCubeMap {
	public:
		virtual ~OpenGLTextureCubeMap() {}

		inline RendererID GetRendererID() const override { return m_RedererId; }
		inline TextureFormat GetFormat() const override { return m_TextureFormat; }
		inline unsigned int GetWidth() const override { return m_Width; }
		inline unsigned int GetHeight() const override { return m_Height; }

		void Bind(unsigned int slot) override;

		OpenGLTextureCubeMap(std::string const& path, bool srgb = true);
		OpenGLTextureCubeMap(TextureFormat format, unsigned int width, unsigned int height, bool srgb = false);

		void UpdateTextureCubaMapLayer(char const* data, unsigned int layer, TextureFormat format = TextureFormat::RGB, unsigned int type = GL_UNSIGNED_BYTE, unsigned int x_offset = 0, unsigned int y_offset = 0, unsigned int width = 0, unsigned int height = 0);

	private:
		RendererID m_RedererId;
		TextureFormat m_TextureFormat;
		unsigned int m_Width;
		unsigned int m_Height;
	};

}