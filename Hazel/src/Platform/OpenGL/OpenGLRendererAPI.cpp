//#include "OpenGLRendererAPI.h"
//#include "Hazel/Renderer/VertexArray.h"
//
//#include <glad/glad.h>
//
//namespace Hazel {
//
//	void OpenGLRendererAPI::Clear(float r, float g, float b, float a)
//	{
//		// �����������Ҫ�Ľ�������������µĲ���ָʾ
//		glClearColor(r, g, b, a);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	}
//
//	void OpenGLRendererAPI::DrawIndexed(VertexArray* vertexArray)
//	{
//		// �������Ҳ��Ҫ�Ľ�����һ���ǻ���������
//		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
//	}
//}