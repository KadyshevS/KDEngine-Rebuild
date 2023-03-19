#pragma once

#include "KDEngine/Renderer/Camera.h"

namespace KDE
{
	class SceneCamera : public Camera 
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		float GetOrthoSize() const { return m_OrthoSize; }

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetOrthoSize(float size);
		void SetViewportSize(uint32_t width, uint32_t height);
	private:
		void RecalculateProjection();
	private:
		float m_OrthoSize = 10.0f;
		float m_OrthoNear = -1.0f, m_OrthoFar = 1.0f;
		float m_AspectRatio = 0.0f;
	};
}