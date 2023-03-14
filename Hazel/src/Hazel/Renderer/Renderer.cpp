#include "Renderer.h"
#include "RenderCommand.h"
#include "RenderCommandQueue.h"

namespace Hazel {

	Renderer* Renderer::s_Instance = new Renderer();

	void Renderer::Clear(float r, float g, float b, float a) {
		HZ_RENDER_4(
			r, g, b, a,
			{
				RendererAPI::Clear(r,g,b,a);
			}
		);
	}

	void Renderer::DrawIndexed(VertexArray* vertexArray) {
		HZ_RENDER_1(
			vertexArray,
			{
				RendererAPI::DrawIndexed(vertexArray);
			}
		);
	}

	void Renderer::WaitAndRender()
	{
		s_Instance->m_CommandQueue.Execute();
	}

}