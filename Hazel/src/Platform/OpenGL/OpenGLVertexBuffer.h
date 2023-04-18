#pragma once
#include "Hazel/Renderer/VertexBuffer.h"
#include "Hazel/Renderer/Buffer.h"

namespace Hazel {

	class OpenGLVertexBuffer :public VertexBuffer {

	public:
		OpenGLVertexBuffer(void* vertices, uint32_t size, VertexBufferUsage usage);
		OpenGLVertexBuffer(uint32_t size, VertexBufferUsage usage);
		virtual ~OpenGLVertexBuffer();

		void Bind() const override;
		void Unbind() const override;
		void SetData(void* buffer, uint32_t size, uint32_t offset = 0) override;

		void SetLayout(const BufferLayout& layout) override;
		const BufferLayout& GetBufferLayout() const override { return m_BufferLayout; }

		virtual uint32_t GetSize() const override { return m_Size; }
		virtual RendererID GetRendererID() const override { return m_RendererID; }

	private:
		uint32_t m_RendererID;
		VertexBufferUsage m_Usage;
		BufferLayout m_BufferLayout;
		uint32_t m_Size;
		Buffer m_LocalData;
	};

}