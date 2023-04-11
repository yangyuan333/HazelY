#include "EditorCamera.h"
#include "Hazel/Input.h"
#include "Hazel/KeyCodes.h"
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Hazel {

	EditorCamera::EditorCamera(glm::mat4 const& projectionMatrix)
		:Camera(projectionMatrix)
	{
		/*
		* Set the inital camera postion and rotation
		*/
		
		m_Rotation = glm::vec3(90.0f, 0.0f, 0.0f); // the only function is to show and save.
		m_FocalPoint = glm::vec3(0.0f);

		glm::vec3 position = glm::vec3{ -5, 5, 5 };
		m_Distance = glm::distance(position, m_FocalPoint);

		// The only factors controlling rotation, m_Yaw and m_Pitch to compute the m_Rotation
		m_Yaw = 3.0f * (float)M_PI / 4.0f;
		m_Pitch = M_PI / 4.0f;

		UpdateCameraView();
	}

	void EditorCamera::OnUpdate(float delta_t)
	{
		/*
		* Set the camera action on the input
		*/

		//  alt

		auto[x_pos, y_pos] = Input::GetMousePostion();
		glm::vec2 delta = glm::vec2{ x_pos - m_InitialMousePosition.x, y_pos - m_InitialMousePosition.y }*0.003f;
		m_InitialMousePosition = glm::vec2{ x_pos, y_pos };

		if (Input::IsKeyPressed(KeyCode::LeftAlt)) {
			if (Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_RIGHT)) {
				MouseZoom(delta.y);
			}
			if (Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT)) {
				MouseRotate(delta);
			}
			if (Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_MIDDLE)) {
				MousePan(delta);
			}
		}
		else {
			if (Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_RIGHT)) {
				//MouseRotate(delta);
				
				if (Input::IsKeyPressed(KeyCode::A)) {
					//MousePan(glm::vec2{ -1.0f, 0.0f });
					m_FocalPoint -= GetRightDirection() * delta_t * KeyPanSpeed;
				}
				else if (Input::IsKeyPressed(KeyCode::D)) {
					m_FocalPoint += GetRightDirection() * delta_t * KeyPanSpeed;
				}
				else if (Input::IsKeyPressed(KeyCode::W)) {
					m_FocalPoint += GetForwardDirection() * delta_t * KeyPanSpeed;
				}
				else if (Input::IsKeyPressed(KeyCode::S)) {
					m_FocalPoint -= GetForwardDirection() * delta_t * KeyPanSpeed;
				}
				else if (Input::IsKeyPressed(KeyCode::Q)) {
					m_FocalPoint += GetUpDirection() * delta_t * KeyPanSpeed;
				}
				else if (Input::IsKeyPressed(KeyCode::E)) {
					m_FocalPoint -= GetUpDirection() * delta_t * KeyPanSpeed;
				}
			}
			if (Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT)) {
				MouseRotate(delta);
			}
			if (Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_MIDDLE)) {
				MousePan(delta);
			}
		}
		
	}

	void EditorCamera::OnEvent(Event& e)
	{
		/*
		* The same as OnUpdate
		*/
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		/*
		* According the Pitch and Yam compute the Rotation
		* The Camera is not allowed to rotate around the z-axis
		*/
		return glm::quat(glm::vec3{ -m_Pitch,-m_Yaw,0 });
	}

	glm::vec3 EditorCamera::GetUpDirection()
	{
		/*
		* Get the Up vector in the world space
		*/
		return glm::toMat3(GetOrientation()) * glm::vec3(0, 1, 0);
	}

	glm::vec3 EditorCamera::GetRightDirection()
	{
		/*
		* Get the Right vector in the world space
		*/
		return glm::toMat3(GetOrientation()) * glm::vec3(1, 0, 0);
	}

	glm::vec3 EditorCamera::GetForwardDirection()
	{
		/*
		* Get the Forward vector in the world space
		*/
		return glm::toMat3(GetOrientation()) * glm::vec3(0, 0, -1);
	}

	void EditorCamera::UpdateCameraView()
	{
		/*
		* According to the Focal Position and the Distance and the Pitch and the Yam to compute the ViewMatrix
		*/
		// 1. Get the Position
		// 2. Get the Rotation
		m_Position = CalculatePosition();
		glm::quat quat = GetOrientation();
		m_Rotation = glm::eulerAngles(quat) * (180.0f / (float)M_PI);
		// the camera's transformation matrix
		m_ViewMatrix = glm::translate(glm::mat4{ 1.0f }, m_Position) * glm::toMat4(quat);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		/*
		* Scroll yam action, about fov, in the future
		*/
		return false;
	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		/*
		* Camera Translation in the Up and Right Direction
		*/
		auto [x_speed, y_speed] = PanSpeed();
		//m_FocalPoint += glm::vec2{ x_speed * delta.x,y_speed * delta.y };
		m_FocalPoint += GetRightDirection() * x_speed * delta.x;
		m_FocalPoint += GetUpDirection() * y_speed * delta.y;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		/*
		* Camera Rotation round the Focal Point
		*/
		glm::vec3 upvector = GetUpDirection();
		float speed = RotationSpeed();
		if (upvector.y >= 0.0f) {
			m_Yaw += delta.x * speed;
			m_Pitch += delta.y * speed;
		}
		else {
			m_Yaw += -1.0f * delta.x * speed;
			m_Pitch += delta.y * speed;
		}
	}

	void EditorCamera::MouseZoom(float delta)
	{
		/*
		* Camera Distance
		*/
		float speed = ZoomSpeed();
		m_Distance -= delta * speed;
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	glm::vec3 EditorCamera::CalculatePosition()
	{
		/*
		* According to the Focal Position and the Rotation to compute the Camera Position in the world
		*/
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

}