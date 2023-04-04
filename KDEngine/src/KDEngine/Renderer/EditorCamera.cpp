#include "kdpch.h"
#include "EditorCamera.h"

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace KDE
{
	EditorCamera::EditorCamera(float fov, float nearClip, float farClip)
		: m_FOV(fov), m_Near(nearClip), m_Far(farClip)
	{
		RecalculateProjection();
	}

	glm::mat4 EditorCamera::GetTransform()
	{
		glm::mat4 t = glm::translate(glm::mat4(1.0f), m_Position);
		glm::mat4 r = glm::toMat4(glm::quat(m_Rotation));

		return t * r;
	}

	void EditorCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}
	void EditorCamera::RecalculateProjection()
	{
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_Near, m_Far);
	}
}