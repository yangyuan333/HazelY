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

		// ����ΪʲôҪ��shader_ptr��������const &�أ�
		const void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		const void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() const override;
		const Ref<IndexBuffer>& GetIndexBuffer() const override;
	private:
		RendererID m_RendererID;
		// ΪʲôҪ���VBO and EBO�أ�
		// VBO�������---�������ݡ�����norm���ݡ�������ɫ���ݿ��Է���VBO�洢
		// EBO��û����⣬Ҳ�ò���ѽ
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffers;
	};

}