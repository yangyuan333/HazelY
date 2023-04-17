#include "RenderCommandQueue.h"
#include "Hazel/Log.h"

namespace Hazel {

	RenderCommandQueue::RenderCommandQueue() {
		m_CommandBuffer = new uint8_t[10 * 1024 * 1024];
		m_CommandBufferPtr = m_CommandBuffer;
		memset(m_CommandBuffer, 0, 10 * 1024 * 1024 * sizeof(uint8_t));
	}

	RenderCommandQueue::~RenderCommandQueue() {
		delete[] m_CommandBuffer;
	}

	void* RenderCommandQueue::Allocate(RenderCommandFn func, uint32_t size) {

		*(RenderCommandFn*)m_CommandBufferPtr = func;
		m_CommandBufferPtr += sizeof(RenderCommandFn);

		*(uint32_t*)m_CommandBufferPtr = size;
		m_CommandBufferPtr += sizeof(uint32_t);

		void* memory = m_CommandBufferPtr;
		m_CommandBufferPtr += size;

		m_RenderCommandCount++;
		return memory;
	}

	void RenderCommandQueue::Execute() {
		HZ_CORE_TRACE("RenderCommandQueue::Execute -- {0} commands, {1} bytes", m_RenderCommandCount, (m_CommandBufferPtr - m_CommandBuffer));

		uint8_t* buffer = m_CommandBuffer;

		for (uint32_t i = 0; i < m_RenderCommandCount; i++)
		{
			RenderCommandFn function = *(RenderCommandFn*)buffer;
			buffer += sizeof(RenderCommandFn);

			uint32_t size = *(uint32_t*)buffer;
			buffer += sizeof(uint32_t);
			function(buffer);
			buffer += size;
		}

		m_CommandBufferPtr = m_CommandBuffer;
		m_RenderCommandCount = 0;
	}

}