#pragma once

#include <glm/glm.hpp>

namespace KDE
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		inline const glm::vec3& GetPosition() const { return m_Position; }
		inline float GetRotation() const { return m_Rotation; }

		inline const glm::mat4& GetProjectionMat() const { return m_ProjectionMat; }
		inline const glm::mat4& GetViewMat() const { return m_ViewMat; }
		inline const glm::mat4& GetViewProjectionMat() const { return m_ViewProjectionMat; }

		void Update();

		void SetPosition(const glm::vec3& position) { m_Position = position; }
		void SetRotation(float rotation) { m_Rotation = rotation; }
	private:
		void CalculateViewProjMat();
	private:
		glm::mat4 m_ProjectionMat;
		glm::mat4 m_ViewMat;
		glm::mat4 m_ViewProjectionMat;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};
}

