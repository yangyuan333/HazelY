#include <Hazel.h>

#include <glm/glm.hpp>

#include <imgui.h>

class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer()
		: Layer("Example") {

	}
	void OnUpdate() override {
		// HZ_INFO("ExampleLayer::Update");
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
		//PushLayer(new ExampleLayer());
		PushOverlayer(new ExampleLayer());
	}
	~Sandbox() override {

	}

};

Hazel::Application* Hazel::CreateApplication() {
	return new Sandbox();
}