#include "OpenGLRendererAPI.h"
#include <glad/glad.h>

namespace Hazel {
	void OpenGLRendererAPI::SetClearColor(glm::vec4 const& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		// �����������Ҫ�Ľ�������������µĲ���ָʾ
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		// �������Ҳ��Ҫ�Ľ�����һ���ǻ���������
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}