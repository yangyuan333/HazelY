#include "OpenGLTexture.h"
#include "OpenGLImage.h"
#include "Hazel/Renderer/Renderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Hazel {

	/// <summary>
	/// Texture2D
	/// </summary>
	/// <param name="format"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="data"></param>
	/// <param name="properties"></param>

	OpenGLTexture2D::OpenGLTexture2D(ImageFormat format, uint32_t width, uint32_t height, const void* data, TextureProperties properties)
		:m_Width(width),m_Height(height),m_Properties(properties)
	{
		/*
		* 或者可以直接捕获=this指针，此时this是const，但是它指向的对象不是const的，因此可以改变
		*/
		m_Image = Image2D::Create(format, width, height, data);
		Ref<Image2D> image = m_Image;
		Renderer::Submit([image, properties]()mutable
			{
				image->Invalidate();
				image.As<OpenGLImage2D>()->CreateSampler(properties);
			}
		);

	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path, TextureProperties properties)
		:m_FilePath(path), m_Properties(properties)
	{
		stbi_set_flip_vertically_on_load(true);
		int width, height, channels;
		if (stbi_is_hdr(path.c_str())) {
			HZ_CORE_INFO("Loading HDR texture {0}, srgb={1}", path, properties.SRGB);
			// 这里好像是有问题来着，如果原始图像只有3个通道，此时期望4个通道读取时，会乱？
			// 出问题的话，优先检查这里
			float* data = stbi_loadf(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
			HZ_CORE_ASSERT(data, "Data Read Failed");
			Buffer buffer((void*)data, Utils::GetImageMemorySize(ImageFormat::RGBA32F, width, height));
			m_Image = Image2D::Create(ImageFormat::RGBA32F, width, height, buffer);
			m_IsHDR = true;
		}
		else {
			HZ_CORE_INFO("Loading texture {0}, srgb={1}", path, properties.SRGB);
			uint8_t* data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
			HZ_CORE_ASSERT(data, "Data Read Failed");
			Buffer buffer((void*)data, Utils::GetImageMemorySize(ImageFormat::RGB, width, height));
			m_Image = Image2D::Create(ImageFormat::RGBA32F, width, height, buffer);
			m_IsHDR = false;
		}

		m_Width = width; m_Height = height; m_Loaded = true;

		/*
		* Texture2D中析构函数处进行了Ref和Submit，因此不会提前析构m_Image
		*/
		Renderer::Submit([=] () mutable
			{
				// 当不使用mutable时，可以使用As绕过const限制，因为const原本限制只针对T*，因此As可以作用，此时再借助于As后的对象即可进行T*指向对象的修改
				m_Image->Invalidate();
				m_Image.As<OpenGLImage2D>()->CreateSampler(properties);
				Buffer& buffer = m_Image->GetBuffer();
				stbi_image_free(buffer.Data);
				buffer = Buffer();
			}
		);
		
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		/*
		* 必须加上mutable，因为const虽然不会限制住T*所指向的对象，但是Ref的->操作符被重载后有const版本，他的返回值是const T*，此时就会起到了双重限制效果
		* lambda表达式捕获了m_Image，也是一个引用，因此m_Image不会被提前释放
		*/
		Ref<Image2D> image = m_Image;
		Renderer::Submit([image]()mutable
			{
				image->Release();
			}
		);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		Renderer::Submit([=]()
			{
				glBindTextureUnit(slot, m_Image.As<OpenGLImage2D>()->GetRendererID());
			}
		);
	}

	uint32_t OpenGLTexture2D::GetMipLevelCount() const
	{
		return Utils::CalculateMipCount(m_Width, m_Height);
	}

	/*
	* 以下三个函数是配合使用的
	*/
	void OpenGLTexture2D::Lock()
	{
		/*
		* 这个有什么用，可能是为了配合多线程使用的
		*/
		m_Locked = true;
	}

	void OpenGLTexture2D::Unlock()
	{
		m_Locked = false;
		Ref<OpenGLTexture2D> instance = this;
		Ref<OpenGLImage2D> image = m_Image.As<OpenGLImage2D>();
		Renderer::Submit([instance, image]() mutable 
			{
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glTextureSubImage2D(
					image->GetRendererID(), 0, 
					0, 0, instance->m_Width, instance->m_Height, 
					Utils::OpenGLImageFormat(image->GetFormat()), GL_UNSIGNED_BYTE, 
					instance->m_Image->GetBuffer().Data);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
				if (instance->GetProperties().GenerateMips) {
					glGenerateTextureMipmap(image->GetRendererID());
				}
			}
		);
	}

	Buffer OpenGLTexture2D::GetWriteableBuffer()
	{
		HZ_CORE_ASSERT(m_Locked, "Texture must be locked!");
		return m_Image->GetBuffer();
	}


	/// <summary>
	/// TextureCubeMap
	/// </summary>
	/// <param name="format"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="data"></param>
	/// <param name="properties"></param>
	/*
	* Texture一定不能先挂掉，不然this就指向空的了
	*/
	OpenGLTextureCubeMap::OpenGLTextureCubeMap(ImageFormat format, uint32_t width, uint32_t height, const void* data, TextureProperties properties)
		:m_Width(width), m_Height(height), m_Format(format), m_Properties(properties)
	{
		if (data) {
			uint32_t size = width * height * Utils::GetImageFormatBPP(format) * 6;
			m_LocalStorage = Buffer::Copy(data, size);
		}
		Renderer::Submit([=]()
			{
				glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);
				glTextureStorage2D(
					m_RendererID,
					properties.GenerateMips ? Utils::CalculateMipCount(m_Width, m_Height) : 1,
					Utils::OpenGLImageInternalFormat(format),
					m_Width, m_Height
				);
				if (m_LocalStorage.Data)
					glTextureSubImage3D(
						m_RendererID, 0,
						0, 0, 0, m_Width, m_Height, 6,
						Utils::OpenGLImageFormat(m_Format), Utils::OpenGLFormatDataType(m_Format),
						m_LocalStorage.Data);
				glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, Utils::OpenGLSamplerFilter(m_Properties.SamplerFilter, m_Properties.GenerateMips));
				glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, Utils::OpenGLSamplerFilter(m_Properties.SamplerFilter, false));
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, Utils::OpenGLSamplerWrap(m_Properties.SamplerWrap));
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, Utils::OpenGLSamplerWrap(m_Properties.SamplerWrap));
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, Utils::OpenGLSamplerWrap(m_Properties.SamplerWrap));
			}
		);
	}

	OpenGLTextureCubeMap::OpenGLTextureCubeMap(const std::string& path, TextureProperties properties)
		: m_FilePath(path), m_Properties(properties)
	{
		/*
		* TODO
		*/
	}

	OpenGLTextureCubeMap::~OpenGLTextureCubeMap()
	{
		Renderer::Submit([=]()
			{
				m_LocalStorage.Release();
				glDeleteTextures(1, &m_RendererID);
			}
		);
	}

	void OpenGLTextureCubeMap::Bind(uint32_t slot) const
	{
		Renderer::Submit([=]()
			{
				glBindTextureUnit(slot, m_RendererID);
			}
		);
	}

	uint32_t OpenGLTextureCubeMap::GetMipLevelCount() const
	{
		return Utils::CalculateMipCount(m_Width, m_Height);
	}

}


/*
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
		
		stbi_set_flip_vertically_on_load(true);

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
					//1,
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
				glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
				//glGenerateMipmap(self->m_RedererId);
				glGenerateTextureMipmap(self->m_RedererId);
				
				glTextureParameteri(self->m_RedererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTextureParameteri(self->m_RedererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTextureParameteri(self->m_RedererId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTextureParameteri(self->m_RedererId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTextureParameterf(self->m_RedererId, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::GetCapabilities().MaxAnisotropy);
				
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

		//stbi_set_flip_vertically_on_load(true);

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

		int faceId = 0;
		std::array<unsigned char*, 6> faces;

		for (size_t i = 0; i < 6; ++i) {
			faces[i] = new unsigned char[m_Width * m_Height * 3];
		}

		for (size_t i = 0; i < 4; ++i) {
			// 横着的四个face数据
			
			for (size_t y = 0; y < m_Height; ++y) {
				// size_t y_offset = m_Height * 2 - int(y) - 1;
				size_t y_offset = m_Height + y;
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
				// size_t y_offset = m_Height * (i+1) - y - 1;
				size_t y_offset = m_Height * i + y;
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
				glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
				//glGenerateMipmap(self->m_RedererId);
				glGenerateTextureMipmap(self->m_RedererId);

				glTextureParameteri(self->m_RedererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTextureParameteri(self->m_RedererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTextureParameteri(self->m_RedererId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTextureParameteri(self->m_RedererId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				// glTextureParameterf(self->m_RedererId, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::GetCapabilities().MaxAnisotropy);

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
	}

}
*/