#pragma once

#include "Hazel/Renderer/VertexArray.h"
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
		const void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		const void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffer() const override;
		const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override;
	private:
		uint32_t m_RendererID;
		// ΪʲôҪ���VBO and EBO�أ�
		// VBO�������---�������ݡ�����norm���ݡ�������ɫ���ݿ��Է���VBO�洢
		// EBO��û����⣬Ҳ�ò���ѽ
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffers;
	};

}