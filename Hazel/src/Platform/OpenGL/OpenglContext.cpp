#include "Hazel/log.h"
#include "OpenglContext.h"
#include <glad/glad.h>

namespace Hazel {

	OpenglContext::OpenglContext(GLFWwindow* windowHandle)
		:m_windowHandle(windowHandle){


	}

	void OpenglContext::init() {
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initailize Glad!");
	}

	void OpenglContext::SwapBuffer() {
		glfwSwapBuffers(m_windowHandle);
	}

}