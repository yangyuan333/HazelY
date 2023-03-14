#include "RenderCommandQueue.h"

namespace Hazel {

	RenderCommandQueue::RenderCommandQueue() {
		m_CommandBuffer = new char[10 * 1024 * 1024];
		m_CommandBufferPtr = m_CommandBuffer;
		memset(m_CommandBuffer, 0, 10 * 1024 * 1024 * sizeof(char));
	}

	RenderCommandQueue::~RenderCommandQueue() {
		delete[] m_CommandBuffer;
	}
	
	void* RenderCommandQueue::Allocate(CommandFunc func, uint32_t size) {
		*(CommandFunc*)m_CommandBufferPtr = func;
		m_CommandBufferPtr += sizeof(CommandFunc);

		*(uint32_t*)m_CommandBufferPtr = size;
		m_CommandBufferPtr += sizeof(uint32_t);

		void* mem = m_CommandBufferPtr;

		m_CommandBufferPtr += size;

		m_RenderCommandCount += 1;
		return mem;
	}

	void RenderCommandQueue::Execute() {

		char* buffer = m_CommandBuffer;
		for (uint32_t idx = 0; idx < m_RenderCommandCount; ++idx) {
			CommandFunc func = *(CommandFunc*)(buffer);
			buffer += sizeof(CommandFunc);

			uint32_t size = *(uint32_t*)buffer;
			buffer += sizeof(uint32_t);

			func(buffer);
			
			buffer += size;
		}

		m_CommandBufferPtr = m_CommandBuffer;
		m_RenderCommandCount = 0;
	}

}