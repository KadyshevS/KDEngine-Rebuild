#include "kdpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace KDE
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMat(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
		m_ViewMat(1.0f)
	{
		KD_PROFILE_FUNCTION();

		m_ViewProjectionMat = m_ProjectionMat * m_ViewMat;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		KD_PROFILE_FUNCTION();

		m_ProjectionMat = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewMat = glm::mat4(1.0f);
		m_ViewProjectionMat = m_ProjectionMat * m_ViewMat;
	}

	void OrthographicCamera::CalculateViewProjMat()
	{
		KD_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::scale(glm::mat4(1.0f), { m_Scale, m_Scale, 1.0f }) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		m_ViewMat = glm::inverse(transform);
		m_ViewProjectionMat = m_ProjectionMat * m_ViewMat;
	}
}