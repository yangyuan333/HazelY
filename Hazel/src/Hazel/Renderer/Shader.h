#pragma once

#include "Hazel/Core.h"
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
// #include <rpcndr.h>

/*
*	Shader得大改
*	1. 改成API适配
*	2. 结合Renderer
*/

namespace Hazel {

	/*
	* TODO: Temp Uniform
	* Uniform和Shader不应该是绑定的
	* 基本调用关系应该是 定义一个Uniform Class数据集合，然后再让Shader Update
	* 目前还未考虑多个shader共用一个uniform集合的高效处理情况
	*/

	enum class UniformType {
		None = 0,
		Float, Float2, Float3, Float4,
		Matrix3x3, Matrix4x4,
		Int32, Uint32
	};

	struct UniformDecl {
		UniformDecl() = default;
		UniformDecl(UniformType type, std::ptrdiff_t offset, std::string name)
			:Type(type),Offset(offset),Name(name)
		{

		}
		UniformType Type;
		std::ptrdiff_t Offset;
		std::string Name;
	};

	class UniformBufferBase {
	public:
		virtual const std::byte* GetBuffer() const = 0;
		virtual const UniformDecl* GetUniforms() const = 0;
		virtual unsigned int GetUniformCount() const = 0;
	};

	template<unsigned int N, unsigned int U>
	class UniformBufferDeclaration :public UniformBufferBase {
	public:
		UniformBufferDeclaration() = default;
		virtual ~UniformBufferDeclaration() = default;
	public:
		virtual const std::byte* GetBuffer() const { return Buffer; }
		virtual const UniformDecl* GetUniforms() const { return Uniforms; }
		virtual unsigned int GetUniformCount() const { return U; }

		template<typename T>
		void Push(const std::string& name, const T& data) {}

		template<>
		void Push(const std::string& name, const int& data) {
			Uniforms[Index++] = UniformDecl{ UniformType::Int32, Cursor, name };
			memcpy(Buffer + Cursor, &data, sizeof(int));
			Cursor += sizeof(int);
		}

		template<>
		void Push(const std::string& name, const unsigned int& data) {
			Uniforms[Index++] = UniformDecl{ UniformType::Uint32, Cursor, name };
			memcpy(Buffer + Cursor, &data, sizeof(unsigned int));
			Cursor += sizeof(unsigned int);
		}

		template<>
		void Push(const std::string& name, const float& data) {
			Uniforms[Index++] = UniformDecl{ UniformType::Float, Cursor, name };
			memcpy(Buffer + Cursor, &data, sizeof(float));
			Cursor += sizeof(float);
		}

		template<>
		void Push(const std::string& name, const glm::vec2& data) {
			Uniforms[Index++] = UniformDecl{ UniformType::Float2, Cursor, name };
			memcpy(Buffer + Cursor, glm::value_ptr(data), sizeof(glm::vec2));
			Cursor += (sizeof(glm::vec2));
		}

		template<>
		void Push(const std::string& name, const glm::vec3& data) {
			Uniforms[Index++] = UniformDecl{ UniformType::Float3, Cursor, name };
			memcpy(Buffer + Cursor, glm::value_ptr(data), sizeof(glm::vec3));
			Cursor += (sizeof(glm::vec3));
		}

		template<>
		void Push(const std::string& name, const glm::vec4& data) {
			Uniforms[Index++] = UniformDecl{ UniformType::Float4, Cursor, name };
			memcpy(Buffer + Cursor, glm::value_ptr(data), sizeof(glm::vec4));
			Cursor += (sizeof(glm::vec4));
		}

		template<>
		void Push(const std::string& name, const glm::mat3x3& data) {
			Uniforms[Index++] = UniformDecl{ UniformType::Matrix3x3, Cursor, name };
			memcpy(Buffer + Cursor, glm::value_ptr(data), sizeof(glm::mat3x3));
			Cursor += (sizeof(glm::mat3x3));
		}

		template<>
		void Push(const std::string& name, const glm::mat4x4& data) {
			Uniforms[Index++] = UniformDecl{ UniformType::Matrix4x4, Cursor, name };
			memcpy(Buffer + Cursor, glm::value_ptr(data), sizeof(glm::mat4x4));
			Cursor += (sizeof(glm::mat4x4));
		}

	private:
		// why not use the vector?
		// the only one reason is quickly. Maybe.
		std::byte Buffer[N];
		UniformDecl Uniforms[U];
		std::ptrdiff_t Cursor{ 0 };
		int Index{ 0 };

	};

	class HAZEL_API Shader {
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual unsigned int GetShader() const = 0;

		virtual void UploadUniformBuffer(const UniformBufferBase& uniformBuffer) = 0;
		// 可要可不要，使用方便
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetUint(const std::string& name, unsigned int value) = 0;

		static Shader* Create(std::string const& filepath);

	public:
		static std::vector<Shader*> s_AllShaders;
	};

}