#include "kdpch.h"
#include "EditorCamera.h"

#include "KDEngine/Input.h"
#include "KDEngine/Codes/MouseCodes.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace KDE
{
	EditorCamera::EditorCamera(float fov, float nearClip, float farClip)
		: m_FOV(fov), m_Near(nearClip), m_Far(farClip), m_FocalPoint(0.0f, 0.0f, 0.0f)
	{
		UpdateProjection();
		UpdateView();
	}

	const glm::mat4& EditorCamera::GetTransform() const
	{
		glm::mat4 t = glm::translate(glm::mat4(1.0f), m_Position);
		glm::mat4 r = glm::toMat4(glm::quat(m_Orientation));

		return t * r;
	}

	void EditorCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);
		UpdateProjection();
	}

	void EditorCamera::UpdateProjection()
	{
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_Near, m_Far);
	}

	void EditorCamera::UpdateView()
	{
		const float radiansYaw = glm::radians(m_Orientation.y);
		const float radiansPitch = glm::radians(m_Orientation.x);

		m_Front = glm::normalize(glm::vec3(
			cos(radiansYaw) * cos(radiansPitch),
			sin(radiansPitch),
			sin(radiansYaw) * cos(radiansPitch)
		));

		m_Right = glm::normalize(glm::cross(m_Front, glm::vec3(0.0f, 1.0f, 0.0f)));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));

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
				const float deltaX = static_cast<float>(m_MousePos.x - Input::GetMouseX());
				const float deltaY = static_cast<float>(m_MousePos.y - Input::GetMouseY());

				m_Orientation.y += deltaX * m_Sensetivity * 0.00001f;
				m_Orientation.x += deltaY * m_Sensetivity * 0.00001f;

				m_Orientation.x = glm::clamp(m_Orientation.x, -89.0f, 89.0f);

				UpdateView();

				m_MousePos = Input::GetMousePosition();
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