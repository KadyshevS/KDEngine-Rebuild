#include "kdpch.h"
#include "EditorCamera.h"

#include "KDEngine/Input.h"
#include "KDEngine/Codes/MouseCodes.h"

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace KDE
{
	EditorCamera::EditorCamera(float fov, float nearClip, float farClip)
		: m_FOV(fov), m_Near(nearClip), m_Far(farClip)
	{
		UpdateProjection();
		UpdateView();
	}

	glm::mat4 EditorCamera::GetTransform()
	{
		glm::mat4 t = glm::translate(glm::mat4(1.0f), m_Position);
		glm::mat4 r = glm::toMat4(glm::quat(m_Orientation));

		return t * r;
	}

	void EditorCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		UpdateProjection();
	}
	void EditorCamera::UpdateProjection()
	{
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_Near, m_Far);
	}
	void EditorCamera::UpdateView()
	{
		m_Front.x = cos(glm::radians(m_Orientation.y)) * cos(glm::radians(m_Orientation.x));
		m_Front.y = sin(glm::radians(m_Orientation.x));
		m_Front.z = sin(glm::radians(m_Orientation.y)) * cos(glm::radians(m_Orientation.x));
		m_Front = glm::normalize(m_Front);

		m_ViewMat = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	void EditorCamera::OnUpdate(Timestep ts)
	{
		if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
		{
			if (m_FirstPressed)
			{
				m_MousePos = Input::GetMousePosition();
				m_FirstPressed = false;
			}
			else if (m_MousePos != Input::GetMousePosition())
			{
				float mouseX = Input::GetMouseX();
				float mouseY = Input::GetMouseY();

				m_Orientation.y += (m_MousePos.x - mouseX) * m_Sensetivity * 0.00001f;
				m_Orientation.x += (m_MousePos.y - mouseY) * m_Sensetivity * 0.00001f;

				m_Orientation.x = std::clamp(m_Orientation.x, -89.0f, 89.0f);

				m_Front = glm::normalize(glm::vec3(
					cos(glm::radians(m_Orientation.y)) * cos(glm::radians(m_Orientation.x)),
					sin(glm::radians(m_Orientation.x)),
					sin(glm::radians(m_Orientation.y)) * cos(glm::radians(m_Orientation.x))
				));

				m_Up = glm::normalize(glm::vec3(
					-sin(glm::radians(m_Orientation.y)),
					cos(glm::radians(m_Orientation.x)),
					cos(glm::radians(m_Orientation.y)) * sin(glm::radians(m_Orientation.x))
				));

				m_Right = glm::normalize(glm::cross(m_Front, m_Up));

				m_MousePos = Input::GetMousePosition();

				UpdateView();
			}
		}
		else
		{
			m_FirstPressed = true;
		}

		glm::vec3 direction = glm::normalize(m_FocalPoint - m_Position);

		glm::vec3 moveDirection = glm::vec3(0.0f);
		if (Input::IsKeyPressed(Key::W))
			moveDirection += glm::rotate(glm::quat(m_Orientation), glm::vec3(0.0f, 0.0f, -1.0f));
		if (Input::IsKeyPressed(Key::S))
			moveDirection -= glm::rotate(glm::quat(m_Orientation), glm::vec3(0.0f, 0.0f, -1.0f));
		if (Input::IsKeyPressed(Key::A))
			moveDirection += glm::rotate(glm::quat(m_Orientation), glm::vec3(-1.0f, 0.0f, 0.0f));
		if (Input::IsKeyPressed(Key::D))
			moveDirection -= glm::rotate(glm::quat(m_Orientation), glm::vec3(-1.0f, 0.0f, 0.0f));

		if (glm::length(moveDirection) > 0.0f)
			moveDirection = glm::normalize(moveDirection);

		m_Position += moveDirection * m_Speed * (float)ts;
		m_FocalPoint += moveDirection * m_Speed * (float)ts;

		UpdateView();
	}
}