#pragma once

#include "Hazel/Core.h"
#include "Hazel/Renderer/Buffer.h"

#include <memory>

namespace Hazel {
	// 这样封装有个缺点
	// 当绑定过的VBO被更改时，不能调用Add，应该调用类似于Update的东西
	class VertexArray {
	public:
		virtual ~VertexArray() {}
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		// 这里为什么要用shader_ptr，而不是const &呢？
		virtual const void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual const void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffer() const = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

		static VertexArray* Create();
	};
}