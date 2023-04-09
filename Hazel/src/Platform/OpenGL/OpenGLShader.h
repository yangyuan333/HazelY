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
		void Bind() override;
		void Unbind() override;
		unsigned int GetShader() const override { return m_RendererID; }

		void UploadUniformBuffer(const UniformBufferBase& uniformBuffer) override;
		// 可要可不要，使用方便
		void SetFloat(const std::string& name, float value) override;
		void SetFloat2(const std::string& name, const glm::vec2& value) override;
		void SetFloat3(const std::string& name, const glm::vec3& value) override;
		void SetFloat4(const std::string& name, const glm::vec4& value) override;
		void SetMat3(const std::string& name, const glm::mat3& value) override;
		void SetMat4(const std::string& name, const glm::mat4& value) override;
		void SetInt(const std::string& name, int value) override;
		void SetUint(const std::string& name, unsigned int value) override;

	private:
		GLenum ShaderTypeFromString(const std::string& type);
		void CompileAndUploadShader();
		void ReadShaderFromFile(const std::string& filepath);

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformUint(const std::string& name, unsigned int value);
		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);
		void UploadUniformMat3(const std::string& name, const glm::mat3& values);
		void UploadUniformMat4(const std::string& name, const glm::mat4& values);

	private:
		unsigned int m_RendererID;
		std::string m_ShaderSource;
	};

}