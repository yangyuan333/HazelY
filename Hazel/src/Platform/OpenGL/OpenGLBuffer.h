#pragma once

#include "Hazel/Renderer/Buffer.h"
#include <glad/glad.h>
#include "Hazel/Core.h"

namespace Hazel {

	// 这种抽象少了控制选项
	// GL_STATIC_DRAW

	class OpenGLVertexBuffer :public VertexBuffer {

	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		void Bind() const override;
		void Unbind() const override;

		void SetLayout(const BufferLayout& layout) override;
		const BufferLayout& GetBufferLayout() const override;

	private:
		uint32_t m_RendererID;
		BufferLayout m_BufferLayout;
	};

	class OpenGLIndexBuffer :public IndexBuffer {

	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		void Bind() const override;
		void Unbind() const override;
		uint32_t GetCount() const override { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

}