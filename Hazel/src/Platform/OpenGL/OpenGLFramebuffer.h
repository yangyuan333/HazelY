#pragma once
#include "Hazel/Renderer/Framebuffer.h"
#include <glad/glad.h>

namespace Hazel {
	class OpenGLFramebuffer :public Framebuffer {
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer() {}
	public:
		void Bind() const override;
		void Unbind() const override;
		void Resize(uint32_t width, uint32_t height, bool forceRecreate = false) override;
		void BindTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const override;
		void ShowFramebufferTexture(uint32_t attachmentIndex = 0) const override;

		uint32_t GetWidth() const override;
		uint32_t GetHeight() const override;

		RendererID GetRendererID() const override;
		RendererID GetColorAttachmentRendererID(int index = 0) const override;
		RendererID GetDepthAttachmentRendererID() const override;

		const FramebufferSpecification& GetSpecification() const override;

	private:
		GLuint buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 , GL_COLOR_ATTACHMENT2 , GL_COLOR_ATTACHMENT3 };
		FramebufferSpecification m_Specification; // 一旦设置就不能再变了
		RendererID m_RendererID = 0;

		std::vector<RendererID> m_ColorAttachments;
		RendererID m_DepthAttachment;

		std::vector<FramebufferTextureFormat> m_ColorAttachmentFormats;
		FramebufferTextureFormat m_DepthAttachmentFormat = FramebufferTextureFormat::None;

		uint32_t m_Width = 0, m_Height = 0;
	};
}