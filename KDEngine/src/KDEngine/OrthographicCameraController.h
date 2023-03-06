#pragma once

#include "Renderer/OrthographicCamera.h"
#include "Core/Timestep.h"

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace KDE
{
	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRation, bool rotation);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		inline OrthographicCamera& GetCamera() { return m_Camera; }
		inline const OrthographicCamera& GetCamera() const { return m_Camera; }
		OrthographicCameraBounds GetBounds() const;
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_Zoom = 1.0f;
		float m_ZoomLimit = 0.1f;
		bool m_Rotation = false;
		OrthographicCameraBounds m_Bounds;
		OrthographicCamera m_Camera;

		float m_TempX = 0.0f, m_TempY = 0.0f;
		bool m_IsDragging = false;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;

		float m_CameraTranslationSpeed = 3.0f;
		float m_CameraRotationSpeed = 180.0f;
		float m_CameraScalingSpeed = 3.0f;
	};
}