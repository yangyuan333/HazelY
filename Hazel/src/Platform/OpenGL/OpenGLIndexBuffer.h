#pragma once

#include "Hazel/Renderer/IndexBuffer.h"

namespace Hazel {

	class OpenGLIndexBuffer :public IndexBuffer {

	public:
		OpenGLIndexBuffer(uint32_t size);
		OpenGLIndexBuffer(void* data, uint32_t size);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void SetData(void* buffer, uint32_t size, uint32_t offset) override;

		virtual uint32_t GetCount() const override;
		virtual uint32_t GetSize() const override;
		virtual RendererID GetRendererID() const override;
	private:
		RendererID m_RendererID = 0;
		uint32_t m_Size;

		Buffer m_LocalData;
	};

}