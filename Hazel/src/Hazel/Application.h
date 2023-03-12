#pragma once
#include "Core.h"
#include "Windows.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/LayerStack.h"
#include "ImGui/ImGuiLayer.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/VertexArray.h"

#include <memory>

namespace Hazel {
	
	// Application�ǵ����ģ�һ��exeֻ��һ��application
	class HAZEL_API Application {
	public:
		Application();
		virtual ~Application();
		void Run();
		
		virtual void OnEvent(Event& e);

		virtual void OnInit() {}
		virtual void OnShutdown() {}
		virtual void OnUpdate() {}


		void PushLayer(Layer* layer);
		void PushOverlayer(Layer* layer);

		static Application& Get() { return *s_Instance; }
		Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		// �����Զ�����
		// һ������ֻ����һ��Application�����һ��
		// �Ȳ��漰�ര��ϵͳ
		std::unique_ptr<Window> m_Window;

		bool m_Running;
		LayerStack m_LayerStack;

		// Ϊʲôֻ��һ���أ�
		// ��Ϊ��Ⱦ��һ��һ�����ģ�����...һ���͹����ˣ�
		//std::shared_ptr<Shader> m_Shader;
		//std::shared_ptr<VertexArray> m_VertexArray;

	private:
		static Application* s_Instance;
	};

	// �����������ⲿ���ã���ʵ�ʲ��ǵģ���Ϊ���.h�ļ��ᱻinclude��sandbox��
	// ���������������sandbox���涨���
	Application* CreateApplication();
}