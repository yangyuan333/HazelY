#pragma once

#include "Hazel/Core.h"

namespace Hazel {

	class GraphicsContext {

	public:
		virtual void init() = 0;
		virtual void SwapBuffer() = 0;
	};

}