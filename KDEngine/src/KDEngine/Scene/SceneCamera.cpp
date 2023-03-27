#include "kdpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace KDE
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float fov, float nearClip, float farClip)
	{
		m_ProjType = ProjectionType::Perspective;
		m_PerspFOV = fov;
		m_PerspNear = nearClip;
		m_PerspFar = farClip;
		RecalculateProjection();
	}
	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjType = ProjectionType::Orthographic;
		m_OrthoSize = size;
		m_OrthoNear = nearClip;
		m_OrthoFar = farClip;
		RecalculateProjection();
	}
	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		if (m_ProjType == ProjectionType::Orthographic)
		{
			float orthoLeft = -m_OrthoSize * m_AspectRatio * 0.5f;
			float orthoRight = -orthoLeft;
			float orthoBottom = -m_OrthoSize * 0.5f;
			float orthoTop = -orthoBottom;

			m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthoNear, m_OrthoFar);
		}
		if (m_ProjType == ProjectionType::Perspective)
		{
			m_Projection = glm::perspective(glm::radians(m_PerspFOV), m_AspectRatio, m_PerspNear, m_PerspFar);
		}
	}
}