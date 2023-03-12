#include "RenderCommandQueue.h"

namespace Hazel {

	RenderCommandQueue::RenderCommandQueue() {
		m_CommandBuffer = new uint32_t[10 * 1028 * 1028];
		m_CommandBufferPtr = m_CommandBuffer;
		memset(m_CommandBuffer, 0, 10 * 1028 * 1028 * sizeof(uint32_t));
	}

	RenderCommandQueue::~RenderCommandQueue() {
		delete[] m_CommandBuffer;
	}

	void RenderCommandQueue::SubmitCommand(CommandFunc func, void* parameters, uint32_t size) {

		uint32_t*& buffer = m_CommandBufferPtr;
		memcpy(buffer, (void*)(&func), sizeof(func));
		buffer += sizeof(func);

		memcpy(buffer, parameters, size);
		buffer += size;

		buffer += (16 - (int)buffer % 16); // 指针是8字节，int是4字节，直接截取了低四字节
		m_RenderCommandCount += 1;

	}

	void RenderCommandQueue::Execute() {

		uint32_t* buffer = m_CommandBuffer;
		for (uint32_t idx = 0; idx < m_RenderCommandCount; ++idx) {
			CommandFunc func = *(CommandFunc*)(buffer);
			buffer += sizeof(CommandFunc);
			buffer += func(buffer);
			buffer += (16 - (int)buffer % 16); // padding 16 byte
		}

		m_CommandBufferPtr = m_CommandBuffer;
		m_RenderCommandCount = 0;
	}

}