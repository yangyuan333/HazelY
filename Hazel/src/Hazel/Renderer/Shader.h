#pragma once

#include "Hazel/Core.h"
#include <string>

/*
*	Shader得大改
*	1. 改成API适配
*	2. 结合Renderer
*/

namespace Hazel {

	class HAZEL_API Shader {
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual unsigned int GetShader() const = 0;
		static Shader* Create(std::string const& filepath);
	};

	/*
	class HAZEL_API Shader {

	public:
		Shader(std::string const& vertexSrc, std::string const& fragmentSrc);
		~Shader();
		
		void Bind() const;
		void Unbind() const;

	private:
		uint32_t m_ShaderId;

	};
	*/
}