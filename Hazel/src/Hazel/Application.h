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
	
	// Application是单例的，一个exe只有一个application
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
		// 便于自动销毁
		// 一个窗口只会与一个Application结合在一起
		// 先不涉及多窗口系统
		std::unique_ptr<Window> m_Window;

		bool m_Running;
		LayerStack m_LayerStack;

		// 为什么只有一个呢？
		// 因为渲染是一个一个来的，所以...一个就够用了，
		//std::shared_ptr<Shader> m_Shader;
		//std::shared_ptr<VertexArray> m_VertexArray;

	private:
		static Application* s_Instance;
	};

	// 看似是用以外部调用，但实际不是的，因为这个.h文件会被include到sandbox上
	// 而这个函数就是在sandbox里面定义的
	Application* CreateApplication();
}