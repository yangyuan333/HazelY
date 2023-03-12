#pragma once

#include "RendererAPI.h"

namespace Hazel {
	// ��������ҲҪ���������Ż������Ƕ���opengl��˵�����ܺ����Ż�����Ϊopengl��ָ���ǵײ���õģ��ϲ��װ�ˣ����ѽ���command������Ż�
	// ���Ƕ�����Ⱦ������˵�����ǿ��Խ����Ż��ģ�������ͬ���ʵ�����һ����Ⱦ�����ٰ��л���ʱ��
	class RenderCommand {
	public:

		inline static void SetClearColor(glm::vec4 const& color) {
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear() {
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(std::shared_ptr<VertexArray>const& vertexArray) {
			s_RendererAPI->DrawIndexed(vertexArray);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}

