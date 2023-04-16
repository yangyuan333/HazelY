#pragma once

#include <GLFW/glfw3.h>
#include "Hazel/Renderer/RendererContext.h"
#include "Hazel/Core.h"

namespace Hazel {

	class OpenGLContext :public RendererContext {
	public:

		OpenGLContext(GLFWwindow* windowHandle);
		virtual ~OpenGLContext();
		void Create() override;
		void BeginFrame() override;
		void SwapBuffers() override;

		void OnResize(uint32_t width, uint32_t height) override;
	private:
		GLFWwindow* m_WindowHandle;

	};

}