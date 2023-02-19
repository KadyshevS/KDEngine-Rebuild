#include <kdpch.h>
#include "OrthographicCameraController.h"

#include "Core.h"

#include "Input.h"
#include "Codes/KeyCodes.h"

namespace KDE
{
	OrthographicCameraController::OrthographicCameraController(float aspectRation, bool rotation)
		: m_Camera( -aspectRation * m_Zoom, aspectRation* m_Zoom, -m_Zoom, m_Zoom ),
		m_AspectRatio(aspectRation),
		m_Rotation(rotation)
	{}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(KD_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts.GetSeconds();
		else if (KDE::Input::IsKeyPressed(KD_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts.GetSeconds();
		if (Input::IsKeyPressed(KD_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts.GetSeconds();
		else if (Input::IsKeyPressed(KD_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts.GetSeconds();

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(KD_KEY_Q))
				m_CameraRotation -= m_CameraRotationSpeed * ts.GetSeconds();
			else if (KDE::Input::IsKeyPressed(KD_KEY_E))
				m_CameraRotation += m_CameraRotationSpeed * ts.GetSeconds();

			m_Camera.SetRotation(m_CameraRotation);
		}
		
		m_Camera.SetPosition(m_CameraPosition);
	}
	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(KD_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(KD_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_Zoom -= e.GetOffsetY() * 0.5f;
		m_Camera.SetProjection( -m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -m_Zoom, m_Zoom );

		return false;
	}
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio -= (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -m_Zoom, m_Zoom);

		return false;
	}
}