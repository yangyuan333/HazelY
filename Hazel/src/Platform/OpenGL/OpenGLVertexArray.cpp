#include "OpenGLVertexArray.h"
#include "Hazel/Renderer/Renderer.h"
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
		Ref<OpenGLVertexArray> instance = this;
		Renderer::Submit([instance]() mutable
			{
				glCreateVertexArrays(1, &instance->m_RendererID);
			}
		);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		RendererID rendererID = m_RendererID;
		Renderer::Submit([rendererID]() mutable
			{
				glDeleteVertexArrays(1, &rendererID);
			}
		);
	}

	void OpenGLVertexArray::Bind() const
	{
		Ref<const OpenGLVertexArray> instance = this;
		Renderer::Submit([instance]() mutable
			{
				glBindVertexArray(instance->m_RendererID);
			}
		);
	}

	void OpenGLVertexArray::Unbind() const
	{
		Renderer::Submit([]()
			{
				glBindVertexArray(0);
			}
		);
	}

	const void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		HZ_CORE_ASSERT(vertexBuffer->GetBufferLayout().GetElements().size(), "Vertex buffer has no layout!");
		
		Bind();
		vertexBuffer->Bind();

		Ref<const OpenGLVertexArray> instance = this;
		Renderer::Submit([instance, vertexBuffer]() mutable
			{
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
			}
		);

		m_VertexBuffers.push_back(vertexBuffer);

		Unbind();
		vertexBuffer->Unbind();
	}

	const void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		Bind();
		indexBuffer->Bind();

		m_IndexBuffers = indexBuffer;

		Unbind();
		indexBuffer->Unbind();
	}

	const std::vector<Ref<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffer() const
	{
		return m_VertexBuffers;
	}

	const Ref<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffers;
	}

}