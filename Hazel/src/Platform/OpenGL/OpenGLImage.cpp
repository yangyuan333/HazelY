#include "OpenGLImage.h"
#include "Hazel/Renderer/Renderer.h"

namespace Hazel {

	OpenGLImage2D::OpenGLImage2D(ImageFormat format, uint32_t width, uint32_t height, Buffer buffer)
		:m_Format(format), m_Width(width), m_Height(height), m_ImageData(buffer)
	{

	}

	OpenGLImage2D::OpenGLImage2D(ImageFormat format, uint32_t width, uint32_t height, const void* data)
		: m_Format(format), m_Width(width), m_Height(height)
	{
		if (data)
			m_ImageData = Buffer::Copy(data, Utils::GetImageMemorySize(format, width, height));
	}

	OpenGLImage2D::~OpenGLImage2D()
	{
		/*
		* 这里不能直接调用，因为会立即释放，需要submit到commandbuffer
		*/
		RendererID rendererID = m_RendererID;
		Renderer::Submit([rendererID] 
			{
				glDeleteTextures(1, &rendererID);
			}
		);
	}

	void OpenGLImage2D::Invalidate()
	{
		/*
		* 删除当前的纹理数据对象，用当前对象的格式和数据再次进行创建
		* TODO: MIPMAP OPTIONAL------DONE
		*/
		if (m_RendererID) {
			glDeleteTextures(1, &m_RendererID);
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(
			m_RendererID,
			m_GenerateMips ? Utils::CalculateMipCount(m_Width, m_Height) : 1,
			Utils::OpenGLImageInternalFormat(m_Format),
			m_Width, m_Height
		);
		if (m_ImageData) {
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTextureSubImage2D(
				m_RendererID,
				0, 0, 0, m_Width, m_Height,
				Utils::OpenGLImageFormat(m_Format),
				Utils::OpenGLFormatDataType(m_Format),
				m_ImageData.Data
			);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			if (m_GenerateMips)
				glGenerateTextureMipmap(m_RendererID);
		}

	}

	void OpenGLImage2D::Release()
	{
		/*
		* 删除对象，释放内存
		*/

		if (m_RendererID) {
			glDeleteTextures(1, &m_RendererID);
			m_RendererID = 0;
		}
		m_ImageData.Release();

	}

	uint32_t OpenGLImage2D::GetWidth() const
	{
		return m_Width;
	}

	uint32_t OpenGLImage2D::GetHeight() const
	{
		return m_Height;
	}

	ImageFormat OpenGLImage2D::GetFormat() const
	{
		return m_Format;
	}

	Buffer OpenGLImage2D::GetBuffer() const
	{
		return m_ImageData;
	}

	Buffer& OpenGLImage2D::GetBuffer()
	{
		// TODO: 在此处插入 return 语句
		return m_ImageData;
	}

	uint64_t OpenGLImage2D::GetHash() const
	{
		return (uint64_t)m_RendererID;
	}

	void OpenGLImage2D::CreateSampler(TextureProperties properties)
	{
		glCreateSamplers(1, &m_SamplerRendererID);
		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_MIN_FILTER, Utils::OpenGLSamplerFilter(properties.SamplerFilter, properties.GenerateMips));
		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_MAG_FILTER, Utils::OpenGLSamplerFilter(properties.SamplerFilter, false));
		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_WRAP_R, Utils::OpenGLSamplerWrap(properties.SamplerWrap));
		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_WRAP_S, Utils::OpenGLSamplerWrap(properties.SamplerWrap));
		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_WRAP_T, Utils::OpenGLSamplerWrap(properties.SamplerWrap));
	}

}