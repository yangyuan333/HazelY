#pragma once

#include "Hazel/Core.h"
#include <string>

namespace Hazel {
	class HAZEL_API Shader {

	public:
		Shader(std::string const& vertexSrc, std::string const& fragmentSrc);
		~Shader();
		
		void Bind() const;
		void Unbind() const;

	private:
		uint32_t m_ShaderId;

	};
}