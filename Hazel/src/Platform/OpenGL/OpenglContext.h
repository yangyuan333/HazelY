#pragma once

#include <GLFW/glfw3.h>
#include "Hazel/Renderer/GraphicsContext.h"

namespace Hazel {

	class OpenglContext :public GraphicsContext {

	public:

		OpenglContext(GLFWwindow* windowHandle);

		void init() override;
		void SwapBuffer() override;
	private:
		GLFWwindow* m_windowHandle;

	};

}