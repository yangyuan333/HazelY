#include "RendererAPI.h"
#include "Renderer.h"

#include <glad/glad.h>

namespace Hazel {
	
	RendererAPIType RendererAPI::s_CurrentRendererAPI = RendererAPIType::OpenGL;

	void RendererAPI::Clear(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RendererAPI::DrawIndexed(VertexArray* vertexArray) {
		vertexArray->Bind();
		glDrawElements(
			GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr
		);
	}

}