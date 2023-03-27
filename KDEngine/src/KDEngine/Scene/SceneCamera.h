#pragma once

#include "KDEngine/Renderer/Camera.h"

namespace KDE
{
	class SceneCamera : public Camera 
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		ProjectionType GetProjectionType() const { return m_ProjType; }
		void SetProjectionType(const ProjectionType& type) { m_ProjType = type; RecalculateProjection(); }

	//	Orthographic
		float GetOrthoSize() const { return m_OrthoSize; }
		float GetOrthoNearClip() const { return m_OrthoNear; }
		float GetOrthoFarClip() const { return m_OrthoFar; }

		void SetOrthoSize(float size) { m_OrthoSize = size; RecalculateProjection(); }
		void SetOrthoNearClip(float nearClip) { m_OrthoNear = nearClip; RecalculateProjection(); }
		void SetOrthoFarClip(float farClip) { m_OrthoFar = farClip; RecalculateProjection(); }

	//	Perspective
		float GetPerspFOV() const { return m_PerspFOV; }
		float GetPerspNearClip() const { return m_PerspNear; }
		float GetPerspFarClip() const { return m_PerspFar; }

		void SetPerspFOV(float fov) { m_PerspFOV = fov; RecalculateProjection(); }
		void SetPerspNearClip(float nearClip) { m_PerspNear = nearClip; RecalculateProjection(); }
		void SetPerspFarClip(float farClip) { m_PerspFar = farClip; RecalculateProjection(); }

		void SetPerspective(float fov, float nearClip, float farClip);
		void SetOrthographic(float size, float nearClip, float farClip);
		void SetViewportSize(uint32_t width, uint32_t height);
	private:
		void RecalculateProjection();
	private:
		ProjectionType m_ProjType = ProjectionType::Orthographic;

		float m_OrthoSize = 10.0f;
		float m_OrthoNear = -1.0f, m_OrthoFar = 1.0f;

		float m_PerspFOV = 45.0f;
		float m_PerspNear = 0.01f, m_PerspFar = 1000.0f;

		float m_AspectRatio = 0.0f;
	};
}