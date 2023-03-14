#include <glad/glad.h>

#include "RendererAPI.h"
#include "VertexArray.h"

namespace Hazel {
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	void RendererAPI::Clear(float r, float g, float b, float a)
	{
		// �����������Ҫ�Ľ�������������µĲ���ָʾ
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RendererAPI::DrawIndexed(VertexArray* vertexArray)
	{
		// �������Ҳ��Ҫ�Ľ�����һ���ǻ���������
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	//RendererAPI* RendererAPI::Create()
	//{
	//	switch (s_API)
	//	{
	//	case Hazel::RendererAPI::API::OpenGL:
	//		return new(OpenGLRendererAPI);
	//	}
	//	HZ_CORE_ASSERT(false, "Unknown RendererAPI type!");
	//}
}