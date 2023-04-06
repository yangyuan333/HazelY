#include "Renderer.h"
#include "RendererAPI.h"
#include "RenderCommandQueue.h"

namespace Hazel {

	Renderer* Renderer::s_Instance = new Renderer();

	void Renderer::Init()
	{
		HZ_RENDER({ RendererAPI::Init(); });
	}

	void Renderer::Clear(float r, float g, float b, float a) {
		HZ_RENDER_4(
			r, g, b, a,
			{
				RendererAPI::Clear(r,g,b,a);
			}
		);
	}

	void Renderer::DrawIndexed(VertexArray* vertexArray, bool depthTest) {
		HZ_RENDER_2(
			vertexArray, depthTest,
			{
				RendererAPI::DrawIndexed(vertexArray, depthTest);
			}
		);
	}

	void Renderer::WaitAndRender()
	{
		s_Instance->m_CommandQueue.Execute();
	}

}