#include "Shader.h"
#include "Hazel/Log.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "RendererAPI.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

namespace Hazel {

	Shader* Shader::Create(std::string const& filepath)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPIType::OpenGL:
		{
			return (new OpenGLShader(filepath));
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI Type!");
		}
	}

}