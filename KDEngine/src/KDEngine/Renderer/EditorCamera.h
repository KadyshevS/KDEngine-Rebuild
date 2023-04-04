#pragma once

#include "KDEngine/Renderer/Camera.h"

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

		void SetPosition(const glm::vec3& position) { m_Position = position; }
		void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; }

		void SetFOV(float fov) { m_FOV = fov; RecalculateProjection(); }
		void SetNearClip(float nearClip) { m_Near = nearClip; RecalculateProjection(); }
		void SetFarClip(float farClip) { m_Far = farClip; RecalculateProjection(); }

		void SetViewportSize(uint32_t width, uint32_t height);
	private:
		void RecalculateProjection();
	private:
		float m_FOV = 45.0f;
		float m_Near = 0.01f, m_Far = 1000.0f;

		glm::vec3 m_Position{0.0f, 0.0f, 0.0f};
		glm::vec3 m_Rotation{0.0f, 0.0f, 0.0f};

		float m_AspectRatio = 0.0f;
	};
}