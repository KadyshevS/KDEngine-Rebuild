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
		m_ViewMat = glm::translate(glm::mat4(1.0f), m_Position) * glm::lookAt(m_Orientation, { 0.0f, 0.0f, 0.0f }, {0.0f, 1.0f, 0.0f});
		m_ViewMat = glm::inverse(m_ViewMat);
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
			else if(m_MousePos != Input::GetMousePosition())
			{
				float mouseX = Input::GetMouseX();
				float mouseY = Input::GetMouseY();

				m_Orientation.y = (m_MousePos.x - mouseX) * m_Sensetivity * 0.00001f;
				m_Orientation.x = (m_MousePos.y - mouseY) * m_Sensetivity * 0.00001f;

				UpdateView();
			}
		}
		else
		{
			m_FirstPressed = true;
		}
	}
}