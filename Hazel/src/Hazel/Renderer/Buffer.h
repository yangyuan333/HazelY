#pragma once

#include <cstdint>
#include "Hazel/Core.h"

// ��û��װ���
// �������Եļ�¼���䲻��װ�
// VAO���ֲ���װ��
// ��������װ��ʲô�����أ�opengl�Ĵ��뻹��ɢ������

namespace Hazel {
	class VertexBuffer {
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);
	};
	class IndexBuffer {
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);
	};
}