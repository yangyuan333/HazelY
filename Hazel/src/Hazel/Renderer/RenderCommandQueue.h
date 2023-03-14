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
		void* Allocate(RenderCommandFn func, unsigned int size);
		void Execute();

	private:
		unsigned char* m_CommandBuffer;
		unsigned char* m_CommandBufferPtr;
		unsigned int m_RenderCommandCount{ 0 };
	};

}