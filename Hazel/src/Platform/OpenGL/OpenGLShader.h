#pragma once

#include "Hazel/Renderer/Shader.h"
#include <glad/glad.h>

namespace Hazel {

	// 得改
	// 1. 不同shader分开
	class OpenGLShader :public Shader {
	public:
		OpenGLShader(std::string const& filepath);
		virtual ~OpenGLShader();
	public:
		GLenum ShaderTypeFromString(const std::string& type);
		void CompileAndUploadShader();
		void Bind() override;
		void Unbind() override;
		unsigned int GetShader() const override { return m_RendererID; }
	private:
		void ReadShaderFromFile(const std::string& filepath);
	private:
		unsigned int m_RendererID;
		std::string m_ShaderSource;
	};

}