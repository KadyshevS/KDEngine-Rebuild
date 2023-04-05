#pragma once

#include "KDEngine/Renderer/Camera.h"
#include "KDEngine/Core/Timestep.h"

#include <glm/glm.hpp>

namespace KDE
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float nearClip, float farClip);
		virtual ~EditorCamera() = default;

		float GetFOV() const { return m_FOV; }
		float GetNearClip() const { return m_Near; }
		float GetFarClip() const { return m_Far; }

		glm::mat4 GetTransform();
		const glm::mat4& GetViewProjectionMat() const { return m_Projection * m_ViewMat; }

		void SetPosition(const glm::vec3& position) { m_Position = position; UpdateView(); }
		void SetOrientation(const glm::vec3& orientation) { m_Orientation = orientation; UpdateView(); }

		void SetFOV(float fov) { m_FOV = fov; UpdateProjection(); }
		void SetNearClip(float nearClip) { m_Near = nearClip; UpdateProjection(); }
		void SetFarClip(float farClip) { m_Far = farClip; UpdateProjection(); }

		void SetViewportSize(uint32_t width, uint32_t height);

		void OnUpdate(Timestep ts);
	private:
		void UpdateProjection();
		void UpdateView();
	private:
		float m_FOV = 45.0f;
		float m_Near = 0.01f, m_Far = 1000.0f;
		float m_Sensetivity = 255.0f;

		bool m_FirstPressed = true;

		glm::mat4 m_ViewMat{ 0.0f };

		glm::vec2 m_MousePos{ 0.0f };

		glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Orientation{ 0.0f, 0.0f, 0.0f };

		float m_AspectRatio = 0.0f;
	};
}