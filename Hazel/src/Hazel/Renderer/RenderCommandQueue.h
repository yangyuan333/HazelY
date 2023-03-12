#pragma once
#include <cstdint>
#include <functional>

namespace Hazel {

	/*
	* 
	*/

	class RenderCommandQueue {
	public:

		using CommandFunc = std::function<uint32_t(void*)>;

		RenderCommandQueue();
		virtual ~RenderCommandQueue();

		void SubmitCommand(CommandFunc func, void* parameters, uint32_t size); // Ìí¼Órendercommand
		void Execute(); // Ö´ÐÐrendercommand

	private:
		uint32_t* m_CommandBuffer;
		uint32_t* m_CommandBufferPtr;
		uint32_t m_RenderCommandCount{ 0 };
	};

}