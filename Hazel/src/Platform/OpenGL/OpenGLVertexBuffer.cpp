#include "OpenGLVertexBuffer.h"
#include "Hazel/Renderer/Renderer.h"
#include <glad/glad.h>

namespace Hazel {

	static GLenum OpenGLUsage(VertexBufferUsage usage)
	{
		switch (usage)
		{
		case VertexBufferUsage::Static:    return GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
		case VertexBufferUsage::Dynamic:   return GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
		}
		HZ_CORE_ASSERT(false, "Unknown vertex buffer usage");
		return 0;
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(void* vertices, uint32_t size, VertexBufferUsage usage)
		:m_Size(size), m_Usage(usage)
	{
		m_LocalData = Buffer::Copy(vertices, size);
		Ref<OpenGLVertexBuffer> instance = this;
		Renderer::Submit([instance]() mutable
			{
				glCreateBuffers(1, &instance->m_RendererID);
				glNamedBufferStorage(instance->m_RendererID, instance->m_Size, instance->m_LocalData.Data, OpenGLUsage(instance->m_Usage));
			}
		);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size, VertexBufferUsage usage)
		:m_Size(size), m_Usage(usage)
	{
		/*
		* ��ò�Ҫ�� = ���в��� this����Ϊ����ֻ�ǲ�����һ�� const ָ�룬�޷�����������������
		*/
		Ref<OpenGLVertexBuffer> instance = this; // ���������ü������������Ķ������Ref��ʽ
		Renderer::Submit([instance]() mutable
			{
				glCreateBuffers(1, &instance->m_RendererID);
				glNamedBufferStorage(instance->m_RendererID, instance->m_Size, nullptr, OpenGLUsage(instance->m_Usage));
			}
		);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		/*
		* �������岶����Ϊ��Ȼ���õ�����������˵��thisָ����Ѿ�UB��
		*/
		GLuint rendererID = m_RendererID;
		Renderer::Submit([rendererID]()
			{
				glDeleteBuffers(1, &rendererID);
			}
		);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		Ref<const OpenGLVertexBuffer> instance = this;
		Renderer::Submit([instance]()
			{
				glBindBuffer(GL_ARRAY_BUFFER, instance->GetRendererID());
			}
		);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		Renderer::Submit([]()
			{
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		);
	}

	void OpenGLVertexBuffer::SetData(void* buffer, uint32_t size, uint32_t offset)
	{
		// ������Ϊ�˽����ݱ�����������ֹbuffer���ⲿ���ͷŵ���
		m_LocalData = Buffer::Copy(buffer, size);
		Ref<OpenGLVertexBuffer> instance = this;
		Renderer::Submit([instance, size, offset]()
			{
				glNamedBufferSubData(instance->m_RendererID, offset, size, instance->m_LocalData.Data);
			}
		);
	}

	void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout)
	{
		m_BufferLayout = layout;
	}

}