#pragma once
#include <cstdint>
#include <functional>

namespace Hazel {

	/*
	* 
	*/

	class RenderCommandQueue {
	public:

		using CommandFunc = std::function<void(void*)>;

		RenderCommandQueue();
		virtual ~RenderCommandQueue();

		void* Allocate(CommandFunc func, uint32_t size);
		void Execute(); // Ö´ÐÐrendercommand

	private:
		char* m_CommandBuffer;
		char* m_CommandBufferPtr;
		uint32_t m_RenderCommandCount{ 0 };
	};

}