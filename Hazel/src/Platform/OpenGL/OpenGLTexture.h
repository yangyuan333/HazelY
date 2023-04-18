#pragma once
#include "Hazel/Renderer/Texture.h"
#include "glad/glad.h"

namespace Hazel {

	class OpenGLTexture2D :public Texture2D {
	public:
		OpenGLTexture2D(ImageFormat format, uint32_t width, uint32_t height, const void* data, TextureProperties properties);
		OpenGLTexture2D(const std::string& path, TextureProperties properties);
		virtual ~OpenGLTexture2D();

		virtual void Bind(uint32_t slot = 0) const;

		virtual Ref<Image2D> GetImage() const override { return m_Image; }

		virtual ImageFormat GetFormat() const override { return m_Image->GetFormat(); }
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		// This function currently returns the expected number of mips based on image size,
		// not present mips in data
		virtual uint32_t GetMipLevelCount() const override;

		virtual void Lock() override;
		virtual void Unlock() override;

		virtual Buffer GetWriteableBuffer() override;

		virtual const std::string& GetPath() const override { return m_FilePath; }

		virtual bool Loaded() const override { return m_Loaded; }

		virtual uint64_t GetHash() const { return m_Image->GetHash(); }
		virtual TextureProperties GetProperties() const { return m_Properties; }

		//virtual ~OpenGLTexture2D() {}
		//inline RendererID GetRendererID() const override { return m_RedererId; }
		//inline TextureFormat GetFormat() const override { return m_TextureFormat; }
		//inline unsigned int GetWidth() const override { return m_Width; }
		//inline unsigned int GetHeight() const override { return m_Height; }

		//void Bind(unsigned int slot) override;
		//
		//OpenGLTexture2D(std::string const& path, TextureFormat innerFormat, TextureFormat outerFormat, bool srgb = true);
		//OpenGLTexture2D(TextureFormat format, unsigned int width, unsigned int height, bool srgb = false);

		//void UpdateTexture2D(char const* data, TextureFormat format = TextureFormat::RGB, unsigned int type = GL_UNSIGNED_BYTE, unsigned int x_offset = 0, unsigned int y_offset = 0, unsigned int width = 0, unsigned int height = 0);

	private:
		Ref<Image2D> m_Image;
		TextureProperties m_Properties;
		uint32_t m_Width, m_Height; // Not Need?

		bool m_IsHDR = false; // use?

		bool m_Locked = false; // use?
		bool m_Loaded = false;

		std::string m_FilePath;

	};

	class OpenGLTextureCubeMap :public TextureCubeMap {
	public:
		OpenGLTextureCubeMap(ImageFormat format, uint32_t width, uint32_t height, const void* data, TextureProperties properties);
		OpenGLTextureCubeMap(const std::string& path, TextureProperties properties);
		virtual ~OpenGLTextureCubeMap();

		virtual void Bind(uint32_t slot = 0) const;

		virtual ImageFormat GetFormat() const { return m_Format; }
		virtual uint32_t GetWidth() const { return m_Width; }
		virtual uint32_t GetHeight() const { return m_Height; }
		// This function currently returns the expected number of mips based on image size,
		// not present mips in data
		virtual uint32_t GetMipLevelCount() const override;

		virtual const std::string& GetPath() const override { return m_FilePath; }

		RendererID GetRendererID() const { return m_RendererID; }

		virtual uint64_t GetHash() const { return (uint64_t)m_RendererID; }
		//virtual ~OpenGLTextureCubeMap() {}

		//inline RendererID GetRendererID() const override { return m_RedererId; }
		//inline TextureFormat GetFormat() const override { return m_TextureFormat; }
		//inline unsigned int GetWidth() const override { return m_Width; }
		//inline unsigned int GetHeight() const override { return m_Height; }

		//void Bind(unsigned int slot) override;

		//OpenGLTextureCubeMap(std::string const& path, TextureFormat innerFormat, TextureFormat outerFormat, bool srgb = true);
		//OpenGLTextureCubeMap(TextureFormat format, unsigned int width, unsigned int height, bool srgb = false);

		//void UpdateTextureCubaMapLayer(char const* data, unsigned int layer, TextureFormat format = TextureFormat::RGB, unsigned int type = GL_UNSIGNED_BYTE, unsigned int x_offset = 0, unsigned int y_offset = 0, unsigned int width = 0, unsigned int height = 0);

	private:
		/*
		* »¹Ã»·â×° ImageCubeMap
		*/
		RendererID m_RendererID;
		ImageFormat m_Format;
		uint32_t m_Width, m_Height;

		TextureProperties m_Properties;

		Buffer m_LocalStorage; // ?

		std::string m_FilePath;

		bool m_IsHDR = false; // use?
	};

}