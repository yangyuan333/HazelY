#include "OpenGLVertexArray.h"
#include <glad/glad.h>

namespace Hazel {

	static uint32_t ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type)
		{
		case Hazel::ShaderDataType::Float:
			return GL_FLOAT;
		case Hazel::ShaderDataType::Float2:
			return GL_FLOAT;
		case Hazel::ShaderDataType::Float3:
			return GL_FLOAT;
		case Hazel::ShaderDataType::Float4:
			return GL_FLOAT;
		case Hazel::ShaderDataType::Mat3:
			return GL_FLOAT;
		case Hazel::ShaderDataType::Mat4:
			return GL_FLOAT;
		case Hazel::ShaderDataType::Int:
			return GL_INT;
		case Hazel::ShaderDataType::Int2:
			return GL_INT;
		case Hazel::ShaderDataType::Int3:
			return GL_INT;
		case Hazel::ShaderDataType::Int4:
			return GL_INT;
		case Hazel::ShaderDataType::Bool:
			return GL_BOOL;
		}
		HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray() {
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	const void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		HZ_CORE_ASSERT(vertexBuffer->GetBufferLayout().GetElements().size(), "Vertex buffer has no layput!");
		
		glBindVertexArray(m_RendererID);

		vertexBuffer->Bind();
		uint32_t ind = 0;
		for (const auto& element : vertexBuffer->GetBufferLayout()) {
			glEnableVertexAttribArray(ind);
			glVertexAttribPointer(
				ind,
				element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type),
				GL_FALSE,
				vertexBuffer->GetBufferLayout().GetStride(),
				(const void*)element.Offset);
			++ind;
		}

		m_VertexBuffers.push_back(vertexBuffer);

		glBindVertexArray(0);
		vertexBuffer->Unbind();
	}

	const void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);

		indexBuffer->Bind();

		m_IndexBuffers = indexBuffer;

		glBindVertexArray(0);
		indexBuffer->Unbind();
	}

	const std::vector<std::shared_ptr<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffer() const
	{
		return m_VertexBuffers;
	}

	const std::shared_ptr<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffers;
	}

}