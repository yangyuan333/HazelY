#include "Renderer.h"
#include "RenderCommand.h"
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

		float data[] = { r,g,b,a };

		s_Instance->m_CommandQueue.SubmitCommand(RenderCommand::Clear, data, 4 * sizeof(float));

	}

	void Renderer::WaitAndRender()
	{
		s_Instance->m_CommandQueue.Execute();
	}

}