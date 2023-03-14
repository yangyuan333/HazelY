#include "RenderCommandQueue.h"
#include "Hazel/Log.h"

namespace Hazel {

	RenderCommandQueue::RenderCommandQueue() {
		m_CommandBuffer = new unsigned char[10 * 1024 * 1024];
		m_CommandBufferPtr = m_CommandBuffer;
		memset(m_CommandBuffer, 0, 10 * 1024 * 1024 * sizeof(unsigned char));
	}

	RenderCommandQueue::~RenderCommandQueue() {
		delete[] m_CommandBuffer;
	}

	void* RenderCommandQueue::Allocate(RenderCommandFn func, unsigned int size) {

		*(RenderCommandFn*)m_CommandBufferPtr = func;
		m_CommandBufferPtr += sizeof(RenderCommandFn);

		*(int*)m_CommandBufferPtr = size;
		m_CommandBufferPtr += sizeof(unsigned int);

		void* memory = m_CommandBufferPtr;
		m_CommandBufferPtr += size;

		m_RenderCommandCount++;
		return memory;
	}

	void RenderCommandQueue::Execute() {
		HZ_CORE_TRACE("RenderCommandQueue::Execute -- {0} commands, {1} bytes", m_RenderCommandCount, (m_CommandBufferPtr - m_CommandBuffer));

		unsigned char* buffer = m_CommandBuffer;

		for (unsigned int i = 0; i < m_RenderCommandCount; i++)
		{
			RenderCommandFn function = *(RenderCommandFn*)buffer;
			buffer += sizeof(RenderCommandFn);

			unsigned int size = *(unsigned int*)buffer;
			buffer += sizeof(unsigned int);
			function(buffer);
			buffer += size;
		}

		m_CommandBufferPtr = m_CommandBuffer;
		m_RenderCommandCount = 0;
	}

}