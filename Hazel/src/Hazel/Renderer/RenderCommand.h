#pragma once

#include "RendererAPI.h"

namespace Hazel {
	// 这个类后续也要继续进行优化，但是对于opengl来说，可能很难优化，因为opengl的指令是底层调用的，上层封装了，很难进行command级别的优化
	// 但是对于渲染流程来说，还是可以进行优化的，例如相同材质的物体一起渲染，减少绑定切换的时间
	class RenderCommand {
	public:

		inline static void SetClearColor(glm::vec4 const& color) {
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear() {
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(std::shared_ptr<VertexArray>const& vertexArray) {
			s_RendererAPI->DrawIndexed(vertexArray);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}

