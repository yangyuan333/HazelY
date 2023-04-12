#pragma once

#include "Hazel/Core.h"
#include <glm/glm.hpp>

namespace Hazel {
	class Camera {
	public:
		Camera() = default;
		Camera(glm::mat4 const& projectionMatrix)
			:m_ProjectionMatrix(projectionMatrix){}
		virtual ~Camera() {};

		void SetProjectionMatrix(glm::mat4 const& projectionMatrix) { m_ProjectionMatrix = projectionMatrix; };
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

		float GetExposure() const { return m_Exposure; }
		float& GetExposure() { return m_Exposure; }

	protected:
		glm::mat4 m_ProjectionMatrix{ 1.0f };
		float m_Exposure = 0.8f; // 物理相机，先不管
	};
}