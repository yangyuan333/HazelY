#include "OpenGLFramebuffer.h"
#include "Hazel/Renderer/Renderer.h"
#include <glad/glad.h>

namespace Hazel {

	namespace Utils {
		static GLenum DataType(GLenum format) {
			// 根据通道类型，返回type？
			switch (format)
			{
			default:
				break;
			}
		}
		static GLenum HazelToOpenGLInnerFramebufferFormat(FramebufferTextureFormat format) {
			switch (format)
			{
			case Hazel::FramebufferTextureFormat::RGBA8:			return GL_RGBA8;
			case Hazel::FramebufferTextureFormat::RGBA16F:			return GL_RGBA16F;
			case Hazel::FramebufferTextureFormat::RGBA32F:			return GL_RGBA32F;
			case Hazel::FramebufferTextureFormat::RG32F:			return GL_RG32F;
			case Hazel::FramebufferTextureFormat::DEPTH32F:			return GL_DEPTH_COMPONENT32F;
			case Hazel::FramebufferTextureFormat::DEPTH24STENCIL8:	return GL_DEPTH24_STENCIL8;
			}
		}
		static GLenum HazelToOpenGLFramebufferAttachmentType(FramebufferTextureFormat format) {
			switch (format)
			{
			case Hazel::FramebufferTextureFormat::DEPTH32F:			return GL_DEPTH_ATTACHMENT;
			case Hazel::FramebufferTextureFormat::DEPTH24STENCIL8:	return GL_DEPTH_STENCIL_ATTACHMENT;
			}
		}
		static unsigned int CalculateMipMapCount(unsigned int width, unsigned int height)
		{
			int levels = 1;
			while ((width | height) >> levels) {
				levels++;
			}
			return levels;
		}
		static void AttachColorTexture(RendererID framebuffer, RendererID texture, int samples, GLenum format, uint32_t width, uint32_t height, int index) {
			// 根据传递的参数进行texture的创建和绑定
			if (samples == 1) {
				glTextureStorage2D(texture, CalculateMipMapCount(width, height), format, width, height);
				glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			else {
				glTextureStorage2DMultisample(texture, samples, format, width, height, GL_FALSE);
			}
			glNamedFramebufferTexture(framebuffer, GL_COLOR_ATTACHMENT0 + index, texture, 0);
		}
		static void AttachDepthTexture(RendererID framebuffer, RendererID texture, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height) {
			// 根据传递的参数进行texture的创建和绑定
			if (samples == 1) {
				glTextureStorage2D(texture, CalculateMipMapCount(width, height), format, width, height);
				glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			else {
				glTextureStorage2DMultisample(texture, samples, format, width, height, GL_FALSE);
			}
			glNamedFramebufferTexture(framebuffer, attachmentType, texture, 0);
		}
		static bool IsDepthFormat(FramebufferTextureFormat format) {
			// 判断是不是深度类型
			switch (format)
			{
			case Hazel::FramebufferTextureFormat::DEPTH32F:
			case Hazel::FramebufferTextureFormat::DEPTH24STENCIL8:
				return true;
			}
			return false;
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////
/// OpenGLFramebuffer /////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		:m_Width(spec.Width), m_Height(spec.Height)
	{
		/*
		*	筛选出color attachment和depth attachment
		*/
		for (auto format : spec.Attachments.Attachments) {
			if (Utils::IsDepthFormat(format.TextureFormat)) {
				m_DepthAttachmentFormat = format.TextureFormat;
			}
			else {
				m_ColorAttachmentFormats.push_back(format.TextureFormat);
			}
		}
		Resize(m_Width, m_Height, true);
	}

	void OpenGLFramebuffer::Bind() const
	{
		HZ_RENDER_S(
			{
				glBindFramebuffer(GL_FRAMEBUFFER, self->m_RendererID);
				glViewport(0, 0, self->m_Width, self->m_Height);
			}
		);
	}

	void OpenGLFramebuffer::Unbind() const
	{
		HZ_RENDER_S(
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height, bool forceRecreate)
	{
		/*
		* delete old opengl data
		* create new opengl data
		* 1. create framebuffer
		* 2. create color attachment
		* 3. create depth attachment
		*/
		m_Width = width;
		m_Height = height;
		HZ_RENDER_S(
			
			// delete old opengl data
			if (self->m_RendererID) {
				glDeleteFramebuffers(1, &self->m_RendererID);
				glDeleteTextures(self->m_ColorAttachments.size(), self->m_ColorAttachments.data());
				glDeleteTextures(1, &self->m_DepthAttachment);
				self->m_ColorAttachments.clear();
				// self->m_DepthAttachment = 0;
			}
			// create new opengl data
			glCreateFramebuffers(1, &self->m_RendererID);
			bool multisample = self->m_Specification.Samples > 1;
			self->m_ColorAttachments.resize(self->m_ColorAttachmentFormats.size());
			glCreateTextures(multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, self->m_ColorAttachments.size(), self->m_ColorAttachments.data());
			for (size_t i = 0; i < self->m_ColorAttachmentFormats.size(); ++i) {

				Utils::AttachColorTexture(
					self->m_RendererID,
					self->m_ColorAttachments[i], self->m_Specification.Samples,
					Utils::HazelToOpenGLInnerFramebufferFormat(self->m_ColorAttachmentFormats[i]),
					self->m_Width, self->m_Height, i);
			}

			Utils::AttachDepthTexture(
				self->m_RendererID,
				self->m_DepthAttachment, self->m_Specification.Samples,
				Utils::HazelToOpenGLInnerFramebufferFormat(self->m_DepthAttachmentFormat),
				Utils::HazelToOpenGLFramebufferAttachmentType(self->m_DepthAttachmentFormat),
				self->m_Width, self->m_Height
			);
			

			if (self->m_ColorAttachments.size() > 1) {
				HZ_CORE_ASSERT(self->m_ColorAttachments.size() <= 4, "Color Attachments are too many!");
				//GLenum buffer[4] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1 ,GL_COLOR_ATTACHMENT2 ,GL_COLOR_ATTACHMENT3 };
				glDrawBuffers(self->m_ColorAttachments.size(), self->buffers);
			}
			else if (self->m_ColorAttachments.size() == 0)
			{
				// Only depth-pass
				glDrawBuffer(GL_NONE);
			}
			glBindFramebuffer(GL_FRAMEBUFFER, self->m_RendererID);
			HZ_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		);
		

	}

	void OpenGLFramebuffer::BindTexture(uint32_t attachmentIndex, uint32_t slot) const
	{
		HZ_RENDER_S2(
			attachmentIndex, slot,
			{
				glBindTextureUnit(slot, self->m_ColorAttachments[attachmentIndex]);
			}
		);
	}

	uint32_t OpenGLFramebuffer::GetWidth() const
	{
		return m_Width;
	}

	uint32_t OpenGLFramebuffer::GetHeight() const
	{
		return m_Height;
	}

	RendererID OpenGLFramebuffer::GetRendererID() const
	{
		return m_RendererID;
	}

	RendererID OpenGLFramebuffer::GetColorAttachmentRendererID(int index) const
	{
		return m_ColorAttachments[index];
	}

	RendererID OpenGLFramebuffer::GetDepthAttachmentRendererID() const
	{
		return m_DepthAttachment;
	}
	const FramebufferSpecification& OpenGLFramebuffer::GetSpecification() const
	{
		// TODO: 在此处插入 return 语句
		return m_Specification;
	}
}