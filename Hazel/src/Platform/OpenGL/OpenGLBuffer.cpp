#include "OpenGLBuffer.h"
#include "Hazel/Renderer/Renderer.h"
#include <glad/glad.h>

namespace Hazel {

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
		HZ_RENDER_S2(
			vertices,size,
			{
				glCreateBuffers(1, &self->m_RendererID);
				glBindBuffer(GL_ARRAY_BUFFER, self->m_RendererID);
				glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		HZ_RENDER_S(
			{
				glDeleteBuffers(1, &self->m_RendererID);
			}
		);
	}

	void OpenGLVertexBuffer::Bind() const {
		HZ_RENDER_S(
			{
				glBindBuffer(GL_ARRAY_BUFFER, self->m_RendererID);
			}
		);
	}

	void OpenGLVertexBuffer::Unbind() const {
		HZ_RENDER(
			{
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		);
	}

	void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout) {
		m_BufferLayout = layout;
	}
	
	const BufferLayout& OpenGLVertexBuffer::GetBufferLayout() const {
		return m_BufferLayout;
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		:m_Count(count) {
		HZ_RENDER_S2(
			indices, count,
			{
				glCreateBuffers(1, &self->m_RendererID);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->m_RendererID);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}
		);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		HZ_RENDER_S(
			{
				glDeleteBuffers(1, &self->m_RendererID);
			}
		);
	}

	void OpenGLIndexBuffer::Bind() const {
		HZ_RENDER_S(
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->m_RendererID);
			}
		);
	}

	void OpenGLIndexBuffer::Unbind() const {
		HZ_RENDER(
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}
		);
	}

}