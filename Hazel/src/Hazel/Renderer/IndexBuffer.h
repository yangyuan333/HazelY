#pragma once

#include "Hazel/Ref.h"
#include "Buffer.h"
#include "RenderTypes.h"

namespace Hazel {
	class IndexBuffer {
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetData(void* buffer, uint32_t size, uint32_t offset = 0) = 0;

		virtual uint32_t GetCount() const = 0;
		virtual uint32_t GetSize() const = 0;
		virtual RendererID GetRendererID() const = 0;

		static Ref<IndexBuffer> Create(uint32_t size);
		static Ref<IndexBuffer> Create(void* data, uint32_t size = 0);
	};
}