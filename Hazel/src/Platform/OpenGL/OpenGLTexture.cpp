#include "OpenGLTexture.h"
#include "Hazel/Renderer/Renderer.h"
#include "stb_image.h"

/*
* TODO:	格式通道不应该由是否是srgb来判断
*/

namespace Hazel {

	static GLenum HazelToOpenGLInnerTextureFormat(TextureFormat format, bool srgb=false)
	{
		if (srgb) {
			switch (format)
			{
			case Hazel::TextureFormat::RGB:     return GL_SRGB8;
			case Hazel::TextureFormat::RGBA:    return GL_SRGB8_ALPHA8;
			case Hazel::TextureFormat::BGR:     return GL_SRGB8;
			case Hazel::TextureFormat::BGRA:    return GL_SRGB8_ALPHA8;
			}
		}
		else {
			switch (format)
			{
			case Hazel::TextureFormat::RGB:     return GL_RGB8;
			case Hazel::TextureFormat::RGBA:    return GL_RGBA8;
			case Hazel::TextureFormat::BGR:     return GL_RGB8;
			case Hazel::TextureFormat::BGRA:    return GL_RGBA8;
			}
		}

		return 0;
	}

	static GLenum HazelToOpenGLOuterTextureFormat(TextureFormat format)
	{
		switch (format)
		{
		case Hazel::TextureFormat::RGB:     return GL_RGB;
		case Hazel::TextureFormat::RGBA:    return GL_RGBA;
		case Hazel::TextureFormat::BGR:     return GL_BGR;
		case Hazel::TextureFormat::BGRA:    return GL_BGRA;
		}

		return 0;
	}

	static unsigned int CalculateMipMapCount(unsigned int width, unsigned int height)
	{
		int levels = 1;
		while ((width | height) >> levels) {
			levels++;
		}
		return levels;
	}

	//////////////////////////////////////////////////////////////////////////////////
	// Texture2D
	//////////////////////////////////////////////////////////////////////////////////
	void OpenGLTexture2D::Bind(unsigned int slot) {
		HZ_RENDER_S1(
			slot,
			{
				glBindTextureUnit(slot, self->m_RedererId);
			}
		);
	}

	OpenGLTexture2D::OpenGLTexture2D(std::string const& path, TextureFormat innerFormat, TextureFormat outerFormat, bool srgb) {
		/*
		* use stb_image.h
		* 不需要进行反转
		*/
		
		int stb_req = 0;
		switch (innerFormat)
		{
		case TextureFormat::RGB: stb_req = STBI_rgb; break;
		case TextureFormat::RGBA: stb_req = STBI_rgb_alpha; break;
		default: stb_req = STBI_default;
		}
		
		int width, height, channels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, stb_req);
		m_Width = width;
		m_Height = height;
		m_TextureFormat = innerFormat;
		HZ_RENDER_S3(
			data, srgb, outerFormat,
			{
				glCreateTextures(GL_TEXTURE_2D, 1, &(self->m_RedererId));
				glTextureStorage2D(
					self->m_RedererId,
					CalculateMipMapCount(self->m_Width, self->m_Height),
					HazelToOpenGLInnerTextureFormat(self->m_TextureFormat, srgb),
					self->m_Width, self->m_Height
				);

				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glTextureSubImage2D(
					self->m_RedererId,0,
					0,0,self->m_Width,self->m_Height,
					HazelToOpenGLOuterTextureFormat(outerFormat),GL_UNSIGNED_BYTE,
					data);
				glGenerateMipmap(self->m_RedererId);
				stbi_image_free(data);
			}
		);

		
	}

	OpenGLTexture2D::OpenGLTexture2D(TextureFormat format, unsigned int width, unsigned int height, bool srgb) {
		m_TextureFormat = format;
		m_Width = width;
		m_Height = height;
		HZ_RENDER_S1(
			srgb,
			{
				glCreateTextures(GL_TEXTURE_2D, 1, &(self->m_RedererId));
				glTextureStorage2D(
					self->m_RedererId,
					CalculateMipMapCount(self->m_Width, self->m_Height),
					HazelToOpenGLInnerTextureFormat(self->m_TextureFormat, srgb),
					self->m_Width, self->m_Height);
			}
		);
	}

	void OpenGLTexture2D::UpdateTexture2D(
		char const* data,
		TextureFormat format, unsigned int type,
		unsigned int x_offset, unsigned int y_offset, unsigned int width, unsigned int height) {
		/*
		* TODO: wait HZ_RENDER_S6
		*/
	}

	//////////////////////////////////////////////////////////////////////////////////
	// TextureCubeMap
	//////////////////////////////////////////////////////////////////////////////////
	void OpenGLTextureCubeMap::Bind(unsigned int slot) {
		HZ_RENDER_S1(
			slot,
			{
				glBindTextureUnit(slot, self->m_RedererId);
			}
		);
	}

	OpenGLTextureCubeMap::OpenGLTextureCubeMap(std::string const& path, TextureFormat innerFormat, TextureFormat outerFormat, bool srgb) {
		/*
		* 默认输入是一张完整的贴图，正方体展开图
		*/

		int stb_req = 0;
		switch (innerFormat)
		{
		case TextureFormat::RGB: stb_req = STBI_rgb; break;
		case TextureFormat::RGBA: stb_req = STBI_rgb_alpha; break;
		default: stb_req = STBI_default;
		}

		int width, height, channel;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channel, stb_req);
		
		m_Width = width / 4;
		m_Height = height / 3;
		m_TextureFormat = innerFormat;
		HZ_CORE_ASSERT(m_Width == m_Height, "Non-square faces!");

		/*
		* 获取6个面的数据
		*/
		int faceId = 0;
		std::array<unsigned char*, 6> faces;
		for (size_t i = 0; i < 4; ++i) {
			// 横着的四个face数据
			faces[i] = new unsigned char[m_Width * m_Height * 3];
			for (size_t y = 0; y < m_Height; ++y) {
				size_t y_offset = y + m_Height;
				for (size_t x = 0; x < m_Width; ++x) {
					faces[faceId][(x + y * m_Width) * 3 + 0] = data[(y_offset * m_Width * 4 + i * m_Width + x) * 3 + 0];
					faces[faceId][(x + y * m_Width) * 3 + 1] = data[(y_offset * m_Width * 4 + i * m_Width + x) * 3 + 1];
					faces[faceId][(x + y * m_Width) * 3 + 2] = data[(y_offset * m_Width * 4 + i * m_Width + x) * 3 + 2];
				}
			}
			++faceId;
		}

		for (size_t i = 0; i < 3; ++i) {
			// 竖着的三个face数据
			if (i == 1)
				continue;
			for (size_t y = 0; y < m_Height; ++y) {
				size_t y_offset = y + m_Height * i;
				for (size_t x = 0; x < m_Width; ++x) {
					faces[faceId][(x + y * m_Width) * 3 + 0] = data[(y_offset * m_Width * 4 + m_Width + x) * 3 + 0];
					faces[faceId][(x + y * m_Width) * 3 + 1] = data[(y_offset * m_Width * 4 + m_Width + x) * 3 + 1];
					faces[faceId][(x + y * m_Width) * 3 + 2] = data[(y_offset * m_Width * 4 + m_Width + x) * 3 + 2];
				}
			}
			++faceId;
		}

		HZ_RENDER_S3(
			faces,srgb, outerFormat,
			{
				glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &(self->m_RedererId));
				glTextureStorage2D(
					self->m_RedererId,
					CalculateMipMapCount(self->m_Width, self->m_Height),
					HazelToOpenGLInnerTextureFormat(self->m_TextureFormat, srgb),
					self->m_Width, self->m_Height);

				//GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				for (size_t i = 0; i < 6; ++i) {
					glTextureSubImage3D(
						self->m_RedererId, 0,
						0,0,i,self->m_Width,self->m_Height,1,
						HazelToOpenGLOuterTextureFormat(outerFormat),GL_UNSIGNED_BYTE,
						faces[i]
					);
				}
				glGenerateMipmap(self->m_RedererId);

				glTextureParameteri(self->m_RedererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTextureParameteri(self->m_RedererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTextureParameteri(self->m_RedererId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTextureParameteri(self->m_RedererId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTextureParameterf(self->m_RedererId, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::GetCapabilities().MaxAnisotropy);

				for (size_t i = 0; i < 6; ++i) {
					delete[] faces[i];
				}
			}
		);
		stbi_image_free(data);
	}

	OpenGLTextureCubeMap::OpenGLTextureCubeMap(TextureFormat format, unsigned int width, unsigned int height, bool srgb) {
		m_TextureFormat = format;
		HZ_RENDER_S3(
			width, height, srgb,
			{
				glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &(self->m_RedererId));
				/*
				* 红宝书上说的有bug，前面说cube map是3D函数，后面又用的是2D函数
				*/
				glTextureStorage2D(
					self->m_RedererId,
					CalculateMipMapCount(width, height),
					HazelToOpenGLInnerTextureFormat(self->m_TextureFormat, srgb),
					width, height);
			}
		);
	}

	void OpenGLTextureCubeMap::UpdateTextureCubaMapLayer(
		char const* data, unsigned int layer, 
		TextureFormat format, unsigned int type, 
		unsigned int x_offset, unsigned int y_offset, unsigned int width, unsigned int height) {
		/*
		* TODO: wait HZ_RENDER_S7
		*/
	}


}