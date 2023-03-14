//#include "OpenGLRendererAPI.h"
//#include "Hazel/Renderer/VertexArray.h"
//
//#include <glad/glad.h>
//
//namespace Hazel {
//
//	void OpenGLRendererAPI::Clear(float r, float g, float b, float a)
//	{
//		// 这里后续还需要改进，给函数添加新的参数指示
//		glClearColor(r, g, b, a);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	}
//
//	void OpenGLRendererAPI::DrawIndexed(VertexArray* vertexArray)
//	{
//		// 这里可能也需要改进，不一定是绘制三角形
//		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
//	}
//}