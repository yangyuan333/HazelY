#pragma once

#include "Hazel/Core.h"
#include "Buffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Hazel/Ref.h"
#include <memory>

namespace Hazel {
	// ������װ�и�ȱ��
	// ���󶨹���VBO������ʱ�����ܵ���Add��Ӧ�õ���������Update�Ķ���
	class VertexArray {
	public:
		virtual ~VertexArray() {}
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		// ����ΪʲôҪ��shader_ptr��������const &�أ�
		virtual const void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual const void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static Ref<VertexArray> Create();
	};
}