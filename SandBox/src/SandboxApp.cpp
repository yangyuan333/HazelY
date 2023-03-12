#include <Hazel.h>
#include <Hazel/ImGui/ImGuiLayer.h>
#include <glm/glm.hpp>

#include <imgui.h>

#include <Hazel/Renderer/Renderer.h>

class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer()
		: Layer("Example") {

	}
	void OnUpdate() override {
		// HZ_INFO("ExampleLayer::Update");
		Hazel::Renderer::GetRenderer()->Clear(0.5, 0.5, 0.5, 1.0);
	}
	void OnEvent(Hazel::Event& event) override {
		// HZ_TRACE("{0}", event.ToString());
	}
	void OnImGuiRender() override
	{
		ImGui::Begin("Test1");
		ImGui::Text("Hello World");
		ImGui::End();
	}

};

class Sandbox: public Hazel::Application {
public:
	Sandbox() {
		
	}
	~Sandbox() override {

	}

	virtual void OnInit() override {
		PushLayer(new ExampleLayer());
		PushOverlayer(new Hazel::ImGuiLayer());
	}
};

Hazel::Application* Hazel::CreateApplication() {
	return new Sandbox();
}