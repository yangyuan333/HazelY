#include "OpenGLShader.h"
#include "Hazel/Renderer/Renderer.h"
#include <glad/glad.h>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include "Hazel/Log.h"

namespace Hazel {

#define UNIFORM_LOGGING 1
	#if UNIFORM_LOGGING
		#define HZ_LOG_UNIFORM(...) HZ_CORE_WARN(__VA_ARGS__)
	#else
		#define HZ_LOG_UNIFORM
#endif

	OpenGLShader::OpenGLShader(std::string const& filepath)
	{
		ReadShaderFromFile(filepath);
		HZ_RENDER_S(
			{
				self->CompileAndUploadShader();
			}
		);
	}

	OpenGLShader::~OpenGLShader()
	{
		HZ_RENDER_S(
			{
				glDeleteProgram(self->m_RendererID);
			}
		);
	}

	GLenum OpenGLShader::ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		return GL_NONE;
	}


	void OpenGLShader::ReadShaderFromFile(const std::string& filepath)
	{
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			m_ShaderSource.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&m_ShaderSource[0], m_ShaderSource.size());
			in.close();
		}
		else
		{
			HZ_CORE_WARN("Could not read shader file {0}", filepath);
		}
	}


	void OpenGLShader::CompileAndUploadShader()
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = m_ShaderSource.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = m_ShaderSource.find_first_of("\r\n", pos);
			HZ_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = m_ShaderSource.substr(begin, eol - begin);
			HZ_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalid shader type specified");

			size_t nextLinePos = m_ShaderSource.find_first_not_of("\r\n", eol);
			pos = m_ShaderSource.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = m_ShaderSource.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? m_ShaderSource.size() - 1 : nextLinePos));
		}

		std::vector<GLuint> shaderRendererIDs;

		GLuint program = glCreateProgram();
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			std::string& source = kv.second;

			GLuint shaderRendererID = glCreateShader(type);
			const GLchar* sourceCstr = (const GLchar*)source.c_str();
			glShaderSource(shaderRendererID, 1, &sourceCstr, 0);

			glCompileShader(shaderRendererID);

			GLint isCompiled = 0;
			glGetShaderiv(shaderRendererID, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shaderRendererID, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shaderRendererID, maxLength, &maxLength, &infoLog[0]);

				HZ_CORE_ERROR("Shader compilation failed:\n{0}", &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shaderRendererID);

				HZ_CORE_ASSERT(false, "Failed");
			}

			shaderRendererIDs.push_back(shaderRendererID);
			glAttachShader(program, shaderRendererID);
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			HZ_CORE_ERROR("Shader compilation failed:\n{0}", &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto id : shaderRendererIDs)
				glDeleteShader(id);
		}

		// Always detach shaders after a successful link.
		for (auto id : shaderRendererIDs)
			glDetachShader(program, id);

		m_RendererID = program;

	}

	void OpenGLShader::Bind()
	{
		HZ_RENDER_S(
			{
				glUseProgram(self->m_RendererID);
			}
		);
	}

	void OpenGLShader::Unbind()
	{
		HZ_RENDER(
			{
				glUseProgram(0);
			}
		);
	}

	void OpenGLShader::UploadUniformBuffer(const UniformBufferBase& uniformBuffer) {

		const std::byte* buffer = uniformBuffer.GetBuffer();
		const UniformDecl* uniforms = uniformBuffer.GetUniforms();
		unsigned int uniformCount = uniformBuffer.GetUniformCount();

		glm::vec2;

		for (size_t i = 0; i < uniformCount; ++i) {
			switch (uniforms[i].Type)
			{
			case UniformType::Int32:		SetInt(uniforms[i].Name, *(int*)(buffer + uniforms[i].Offset)); break;
			case UniformType::Uint32:		SetUint(uniforms[i].Name, *(unsigned int*)(buffer + uniforms[i].Offset)); break;
			case UniformType::Float:		SetFloat(uniforms[i].Name, *(float*)(buffer + uniforms[i].Offset)); break;
			case UniformType::Float2:		SetFloat2(uniforms[i].Name, *(glm::vec2*)(buffer + uniforms[i].Offset)); break;
			case UniformType::Float3:		SetFloat3(uniforms[i].Name, *(glm::vec3*)(buffer + uniforms[i].Offset)); break;
			case UniformType::Float4:		SetFloat4(uniforms[i].Name, *(glm::vec4*)(buffer + uniforms[i].Offset)); break;
			case UniformType::Matrix3x3:	SetMat3(uniforms[i].Name, *(glm::mat3x3*)(buffer + uniforms[i].Offset)); break;
			case UniformType::Matrix4x4:	SetMat4(uniforms[i].Name, *(glm::mat4x4*)(buffer + uniforms[i].Offset)); break;
			default:						break;
			}
		}

	}

	// 可要可不要，使用方便
	void OpenGLShader::SetInt(const std::string& name, int value) {
		HZ_RENDER_S2(
			name, value,
			{
				self->UploadUniformInt(name,value);
			}
		);
	}

	void OpenGLShader::SetUint(const std::string& name, unsigned int value) {
		HZ_RENDER_S2(
			name, value,
			{
				self->UploadUniformUint(name,value);
			}
		);
	}
	
	void OpenGLShader::SetFloat(const std::string& name, float value) {
		HZ_RENDER_S2(
			name, value,
			{
				self->UploadUniformFloat(name,value);
			}
		);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value) {
		HZ_RENDER_S2(
			name, value,
			{
				self->UploadUniformFloat2(name,value);
			}
		);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value) {
		HZ_RENDER_S2(
			name, value,
			{
				self->UploadUniformFloat3(name,value);
			}
		);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value) {
		HZ_RENDER_S2(
			name, value,
			{
				self->UploadUniformFloat4(name,value);
			}
		);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& value) {
		HZ_RENDER_S2(
			name, value,
			{
				self->UploadUniformMat3(name,value);
			}
		);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value) {
		HZ_RENDER_S2(
			name, value,
			{
				self->UploadUniformMat4(name,value);
			}
		);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value) {
		glUseProgram(m_RendererID);
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location != -1)
			glUniform1i(location, value);
		else
			HZ_LOG_UNIFORM("Uniform '{0}' not found!", name);
	}

	void OpenGLShader::UploadUniformUint(const std::string& name, unsigned int value) {
		glUseProgram(m_RendererID);
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location != -1)
			glUniform1ui(location, value);
		else
			HZ_LOG_UNIFORM("Uniform '{0}' not found!", name);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value) {
		glUseProgram(m_RendererID);
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location != -1)
			glUniform1f(location, value);
		else
			HZ_LOG_UNIFORM("Uniform '{0}' not found!", name);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values) {
		glUseProgram(m_RendererID);
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location != -1)
			glUniform2f(location, values[0], values[1]);
		else
			HZ_LOG_UNIFORM("Uniform '{0}' not found!", name);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values) {
		glUseProgram(m_RendererID);
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location != -1)
			glUniform3f(location, values[0], values[1], values[2]);
		else
			HZ_LOG_UNIFORM("Uniform '{0}' not found!", name);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values) {
		glUseProgram(m_RendererID);
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location != -1)
			glUniform4f(location, values[0], values[1], values[2], values[3]);
		else
			HZ_LOG_UNIFORM("Uniform '{0}' not found!", name);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& values) {
		glUseProgram(m_RendererID);
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location != -1)
			glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(values));
		else
			HZ_LOG_UNIFORM("Uniform '{0}' not found!", name);
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& values) {
		glUseProgram(m_RendererID);
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location != -1)
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(values));
		else
			HZ_LOG_UNIFORM("Uniform '{0}' not found!", name);
	}



}