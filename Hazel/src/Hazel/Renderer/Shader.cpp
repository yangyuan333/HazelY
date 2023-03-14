#include "Shader.h"
#include "Hazel/Log.h"
#include "Hazel/Renderer/Renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

namespace Hazel {
	Shader::Shader(std::string const& vertexSrc, std::string const& fragmentSrc) {

		HZ_RENDER_S2(
			vertexSrc, fragmentSrc,
			{
				GLuint vertexId = glCreateShader(GL_VERTEX_SHADER);
				GLuint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);

				const GLchar * source = (const GLchar*)(vertexSrc.c_str());
				glShaderSource(vertexId, 1, &source, 0);
				glCompileShader(vertexId);
				// check compile is yes or not
				GLint isCompiled = 0;
				glGetShaderiv(vertexId, GL_COMPILE_STATUS, &isCompiled);
				if (isCompiled == GL_FALSE) {
					GLint maxLength = 0;
					glGetShaderiv(vertexId, GL_INFO_LOG_LENGTH, &maxLength);
					std::vector<GLchar> infoLog(maxLength);
					glGetShaderInfoLog(vertexId, maxLength, &maxLength, &infoLog[0]);
					glDeleteShader(vertexId);

					HZ_CORE_ERROR("{0}", infoLog.data());
					HZ_CORE_ASSERT(false, "Vertex shader compilation failure!");
				}

				source = (const GLchar*)(fragmentSrc.c_str());
				glShaderSource(fragmentId, 1, &source, 0);
				glCompileShader(fragmentId);
				// check compile is yes or not
				glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &isCompiled);
				if (isCompiled == GL_FALSE) {
					GLint maxLength = 0;
					glGetShaderiv(fragmentId, GL_INFO_LOG_LENGTH, &maxLength);
					std::vector<GLchar> infoLog(maxLength);
					glGetShaderInfoLog(fragmentId, maxLength, &maxLength, &infoLog[0]);
					glDeleteShader(fragmentId);

					HZ_CORE_ERROR("{0}", infoLog.data());
					HZ_CORE_ASSERT(false, "Fragment shader compilation failure!");

				}

				self->m_ShaderId = glCreateProgram();
				glAttachShader(self->m_ShaderId, vertexId);
				glAttachShader(self->m_ShaderId, fragmentId);
				glLinkProgram(self->m_ShaderId);
				// check link
				GLint isLinked = 0;
				glGetProgramiv(self->m_ShaderId, GL_LINK_STATUS, (int*)&isLinked);
				if (isLinked == GL_FALSE) {
					GLint maxLength = 0;
					glGetProgramiv(self->m_ShaderId, GL_INFO_LOG_LENGTH, &maxLength);
					std::vector<GLchar> infoLog(maxLength);
					glGetProgramInfoLog(self->m_ShaderId, maxLength, &maxLength, &infoLog[0]);
					glDeleteShader(vertexId);
					glDeleteShader(fragmentId);

					HZ_CORE_ERROR("{0}", infoLog.data());
					HZ_CORE_ASSERT(false, "Shader link failure!");
				}

				glDetachShader(self->m_ShaderId, vertexId);
				glDetachShader(self->m_ShaderId, fragmentId);
			}
		);
	}

	Shader::~Shader() {
		HZ_RENDER_S(
			{
				glDeleteProgram(self->m_ShaderId);
			}
		);
	}

	void Shader::Bind()const {
		HZ_RENDER_S(
			{
				glUseProgram(self->m_ShaderId);

			}
		);
	}

	void Shader::Unbind() const {
		HZ_RENDER(
			{
				glUseProgram(0);
			}
		);
	}
}