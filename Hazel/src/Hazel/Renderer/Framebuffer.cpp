#include "Framebuffer.h"
#include "Hazel/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Hazel {

	FramebufferPool* FramebufferPool::s_Instance = new FramebufferPool;

	Framebuffer* Framebuffer::Create(const FramebufferSpecification& spec)
	{
		// TODO: 在此处插入 return 语句
		Hazel::Framebuffer* result = nullptr;
		switch (RendererAPI::GetAPI())
		{
		case RendererAPIType::None: return nullptr;
		case RendererAPIType::OpenGL: result = new OpenGLFramebuffer(spec);
		}
		FramebufferPool::GetGlobal()->Add(result);
		return result;

	}
	FramebufferPool::FramebufferPool(uint32_t maxFBs)
	{
	}
	FramebufferPool::~FramebufferPool()
	{
	}
	std::weak_ptr<Framebuffer> FramebufferPool::AllocateBuffer()
	{
		// 没啥用
		return std::weak_ptr<Framebuffer>();
	}
	void FramebufferPool::Add(Framebuffer* framebuffer)
	{
		m_Pool.push_back(framebuffer);
	}
}