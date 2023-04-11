#pragma once
#include <iostream>
#include "Hazel/Renderer/Camera.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/MouseEvent.h"
#include <glm/gtc/quaternion.hpp>

namespace Hazel {
	class EditorCamera :public Camera {
	public:
		EditorCamera(glm::mat4 const & projectionMatrix);
		virtual~EditorCamera() {}

	public:
		void OnUpdate(float delta_t); // Update����ѯ
		void OnEvent(Event& e); // Event���ж�

	public:
		float GetDistance() const { return m_Distance; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjection() const { return m_ProjectionMatrix * m_ViewMatrix; }

	public:
		void SetDistance(float distance) { m_Distance = distance; }
		void SetViewportSize(uint32_t width, uint32_t height) { m_ViewportWidth = width; m_ViewportHeight = height; }
	
	public:
		glm::quat GetOrientation() const;
		glm::vec3 GetUpDirection();
		glm::vec3 GetRightDirection();
		glm::vec3 GetForwardDirection();
		const glm::vec3& GetPosition() const { return m_Position; }
		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }
	private:
		void UpdateCameraView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition();

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

	private:

		inline static float KeyPanSpeed = 2.5f;

		glm::mat4 m_ViewMatrix{ 1.0f };
		
		// �������̬��Ϣ
		glm::vec3 m_Rotation;
		glm::vec3 m_FocalPoint;
		glm::vec3 m_Position;
		float m_Distance;
		float m_Pitch, m_Yaw;

		// ��һ֡�������Ϣ
		glm::vec2 m_InitialMousePosition;
		glm::vec3 m_InitialFocalPoint, m_InitialRotation;


		uint32_t m_ViewportWidth = 1280, m_ViewportHeight = 720;
	};
}