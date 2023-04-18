#include "OpenGLIndexBuffer.h"
#include "Hazel/Renderer/Renderer.h"
#include <glad/glad.h>

namespace Hazel {

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t size)
		:m_Size(size)
	{
		Ref<OpenGLIndexBuffer> instance = this;
		Renderer::Submit([instance]() mutable
			{
				glCreateBuffers(1, &instance->m_RendererID);
				glNamedBufferStorage(instance->m_RendererID, instance->m_Size, nullptr, GL_DYNAMIC_STORAGE_BIT);
			}
		);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(void* data, uint32_t size)
		:m_Size(size)
	{
		m_LocalData = Buffer::Copy(data, size);
		Ref<OpenGLIndexBuffer> instance = this;
		Renderer::Submit([instance]() mutable
			{
				glCreateBuffers(1, &instance->m_RendererID);
				glNamedBufferStorage(instance->m_RendererID, instance->m_Size, instance->m_LocalData.Data, GL_DYNAMIC_STORAGE_BIT);
			}
		);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		RendererID rendererId = m_RendererID;
		Renderer::Submit([rendererId]() mutable
			{
				glDeleteBuffers(1, &rendererId);
			}
		);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		Ref<const OpenGLIndexBuffer> instance = this;
		Renderer::Submit([instance]() mutable
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, instance->m_RendererID);
			}
		);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		Renderer::Submit([]()
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}
		);
	}

	void OpenGLIndexBuffer::SetData(void* buffer, uint32_t size, uint32_t offset)
	{
		m_LocalData = Buffer::Copy(buffer, size);
		Ref<OpenGLIndexBuffer> instance = this;
		Renderer::Submit([instance, size, offset]() mutable
			{
				glNamedBufferSubData(instance->m_RendererID, offset, size, instance->m_LocalData.Data);
			}
		);
	}

	uint32_t OpenGLIndexBuffer::GetCount() const
	{
		return m_Size / sizeof(uint32_t);
	}

	uint32_t OpenGLIndexBuffer::GetSize() const
	{
		return m_Size;
	}

	RendererID OpenGLIndexBuffer::GetRendererID() const
	{
		return m_RendererID;
	}

}