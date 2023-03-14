#include "Renderer.h"
#include "RenderCommandQueue.h"

namespace Hazel {

	//void Renderer::BeginScene()
	//{
	//	// set the uniform parameters, like light or camera or enviroment map.
	//}
	//void Renderer::EndScene()
	//{
	//}
	//void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	//{
	//	vertexArray->Bind();
	//	RenderCommand::DrawIndexed(vertexArray);
	//}

	Renderer* Renderer::s_Instance = new Renderer();

	void Renderer::Clear(float r, float g, float b, float a) {
		// 不在需要rendercommand了，之前的command是为了计算参数size，辅助renderapi，现在用宏进行了替换
		float data[] = { r,g,b,a };

		HZ_RENDER_4(
			r, g, b, a,
			{ RendererAPI::Clear(r, g, b, a); }
		);

	}

	void Renderer::DrawIndexed(VertexArray* vertexArray) {

		HZ_RENDER_1(
			vertexArray,
			{ RendererAPI::DrawIndexed(vertexArray); }
		);

	}

	void Renderer::WaitAndRender()
	{
		s_Instance->m_CommandQueue.Execute();
	}

	void* Renderer::Submit(RenderCommandFn fn, unsigned int size) {
		return s_Instance->m_CommandQueue.Allocate(fn, size);
	}

}