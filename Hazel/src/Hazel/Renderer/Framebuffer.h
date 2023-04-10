#pragma once

#include "Hazel/Core.h"
#include "Hazel/Renderer/RendererAPI.h"
#include <vector>
#include <glm/glm.hpp>
#include <memory>

namespace Hazel {

	enum class FramebufferTextureFormat {
		None = 0,
		// Color
		RGBA8	= 1,
		RGBA16F = 2,
		RGBA32F = 3,
		RG32F	= 4,

		// Depth
		DEPTH32F = 5,
		DEPTH24STENCIL8 = 6,

		// Default
		Depth = DEPTH24STENCIL8
	};

	/*
	* Specification: Texture, Attachment, Framebuffer
	*/

	struct FramebufferTextureSpecification {
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			:TextureFormat(format) {}
		FramebufferTextureFormat TextureFormat;
		// TODO: filtering and wrap, now they are const
	};

	struct FramebufferAttachmentSpecification {
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(const std::initializer_list<FramebufferTextureSpecification>& attachments)
			: Attachments(attachments) {}
		std::vector<FramebufferTextureSpecification> Attachments;
	};
	
	struct FramebufferSpecification {
		FramebufferAttachmentSpecification Attachments;

		uint32_t Width = 1280;
		uint32_t Height = 720;
		glm::vec4 ClearColor{ 0.1f,0.1f,0.1f,1.0f };
		uint32_t Samples = 1;

		bool NoResize = false;

		// 目前完全不知道干嘛用的，甚至都没有封装交换framebuffer的函数，唯一解释最后额外补一轮空draw
		bool SwapChainTarget = false;

	};

	class Framebuffer {
	public:
		virtual ~Framebuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Resize(uint32_t width, uint32_t height, bool forceRecreate = false) = 0;
		virtual void BindTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const = 0;
		virtual void ShowFramebufferTexture(uint32_t attachmentIndex = 0) const = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual RendererID GetRendererID() const = 0;
		virtual RendererID GetColorAttachmentRendererID(int index = 0) const = 0;
		virtual RendererID GetDepthAttachmentRendererID() const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Framebuffer* Create(const FramebufferSpecification& spec);

	};

	class FramebufferPool final
	{
	public:
		FramebufferPool(uint32_t maxFBs = 32);
		~FramebufferPool();

		std::weak_ptr<Framebuffer> AllocateBuffer();
		void Add(Framebuffer* framebuffer);

		std::vector<Framebuffer*>& GetAll() { return m_Pool; }
		const std::vector<Framebuffer*>& GetAll() const { return m_Pool; }

		inline static FramebufferPool* GetGlobal() { return s_Instance; }
	private:
		std::vector<Framebuffer*> m_Pool;

		static FramebufferPool* s_Instance;
	};

}