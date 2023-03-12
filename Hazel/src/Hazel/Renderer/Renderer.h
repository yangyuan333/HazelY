#pragma once

#include "RenderCommand.h"

namespace Hazel {

	class Renderer {
	public:
		static void BeginScene(); // TODO
		static void EndScene(); // TODO
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray); // TODO
		
	public:
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};

}