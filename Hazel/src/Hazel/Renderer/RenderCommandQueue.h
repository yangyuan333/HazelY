#pragma once
#include <cstdint>
#include <functional>

namespace Hazel {

	class RenderCommandQueue {
	public:
		typedef void(*RenderCommandFn)(void*);

		RenderCommandQueue();
		virtual ~RenderCommandQueue();

	public:
		void* Allocate(RenderCommandFn func, uint32_t size);
		void Execute();

	private:
		uint8_t * m_CommandBuffer;
		uint8_t * m_CommandBufferPtr;
		uint32_t m_RenderCommandCount{ 0 };
	}; 

}