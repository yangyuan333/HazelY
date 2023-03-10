#pragma once

#include <cstdint>
#include "Hazel/Core.h"

// 还没封装完吧
// 顶点属性的记录分配不封装嘛？
// VAO这种不封装？
// 那这样封装有什么意义呢？opengl的代码还是散在外面

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