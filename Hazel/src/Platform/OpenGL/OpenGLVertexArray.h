#pragma once

#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/RenderTypes.h"
#include "Hazel/Core.h"
#include <memory>
#include <vector>

namespace Hazel {

	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();
		void Bind() const override;
		void Unbind() const override;

		// 这里为什么要用shader_ptr，而不是const &呢？
		const void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		const void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() const override;
		const Ref<IndexBuffer>& GetIndexBuffer() const override;
	private:
		RendererID m_RendererID;
		// 为什么要多个VBO and EBO呢？
		// VBO可以理解---顶点数据。。。norm数据。。。颜色数据可以分离VBO存储
		// EBO真没法理解，也用不了呀
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffers;
	};

}