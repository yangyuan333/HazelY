#include "Renderer.h"
#include "RenderCommand.h"

namespace Hazel {
	void Renderer::BeginScene()
	{
		// set the uniform parameters, like light or camera or enviroment map.
	}
	void Renderer::EndScene()
	{

	}
	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}