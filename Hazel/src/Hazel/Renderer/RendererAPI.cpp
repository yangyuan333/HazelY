#include "RendererAPI.h"
#include "Renderer.h"

#include <glad/glad.h>

namespace Hazel {
	
	static void OpenGLLogMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
			HZ_CORE_ERROR("{0}", message);
	}

	RendererAPIType RendererAPI::s_CurrentRendererAPI = RendererAPIType::OpenGL;

	void RendererAPI::Init() {
		glDebugMessageCallback(OpenGLLogMessage, nullptr);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		// 逆时针正面，这是个默认参数了
		glFrontFace(GL_CCW);
		// 透明度混合
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		auto& caps = GetCapabilities();
		caps.Vendor = (const char *)glGetString(GL_VENDOR);
		caps.Renderer = (const char*)glGetString(GL_RENDERER);
		caps.Version = (const char *)glGetString(GL_VERSION);
		glGetIntegerv(GL_MAX_SAMPLES, &caps.MaxSamples);
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &caps.MaxAnisotropy);

	}

	void RendererAPI::Shutdown() {

	}

	void RendererAPI::Clear(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RendererAPI::DrawIndexed(VertexArray* vertexArray, bool depthTest) {
		if (depthTest) {
			glEnable(GL_DEPTH_TEST);
		}
		else {
			glDisable(GL_DEPTH_TEST);
		}
		vertexArray->Bind();
		glDrawElements(
			GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr
		);
	}

}