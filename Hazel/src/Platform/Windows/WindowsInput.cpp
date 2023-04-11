#include "WindowsInput.h"
#include "Hazel/Application.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hazel {

	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(KeyCode keycode) {
		// 这里后续可能会重构
		// 目前是ok的，因为一个application目前只会有一个window
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(keycode));
		return state == GLFW_PRESS || state == GLFW_RELEASE;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button) {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePostionImpl() {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { float(xpos),float(ypos) };
	}

	float WindowsInput::GetMouseXImpl() {
		auto [xpos, ypos] = GetMousePostionImpl();
		return (float)(xpos);
	}

	float WindowsInput::GetMouseYImpl() {
		auto [xpos, ypos] = GetMousePostionImpl();
		return (float)(ypos);
	}

}