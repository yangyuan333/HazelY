#pragma once

#include "Hazel/Core.h"

namespace Hazel {

	enum class RendererAPI {
		None = 0,
		OpenGL = 1
	};

	class Renderer {
	public:
		static RendererAPI GetRendererAPI() { return s_RendererAPI; }
	private:
		static RendererAPI s_RendererAPI;
	};

}