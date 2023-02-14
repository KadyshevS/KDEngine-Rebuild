#include "kdpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace KDE
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMat( glm::ortho(left, right, bottom, top, -1.0f, 1.0f) ),
		m_ViewMat(1.0f)
	{
		m_ViewProjectionMat = m_ProjectionMat * m_ViewMat;
	}

	void OrthographicCamera::CalculateViewProjMat()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0.0f, 0.0f, 1.0f));

		m_ViewMat = glm::inverse(transform);
		m_ViewProjectionMat = m_ProjectionMat * m_ViewMat;
	}
}