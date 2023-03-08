#include <kdpch.h>
#include "OrthographicCameraController.h"

#include "Core.h"

#include "Input.h"
#include "Codes/KeyCodes.h"
#include "Codes/MouseCodes.h"

#include "imgui.h"

namespace KDE
{
	OrthographicCameraController::OrthographicCameraController(float aspectRation, bool rotation)
		: m_Camera(-aspectRation * m_Zoom, aspectRation* m_Zoom, -m_Zoom, m_Zoom),
		m_AspectRatio(aspectRation),
		m_Bounds({ -m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -m_Zoom, m_Zoom }),
		m_Rotation(rotation)
	{}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(KD_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * m_Zoom * ts.GetSeconds();
		else if (KDE::Input::IsKeyPressed(KD_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * m_Zoom * ts.GetSeconds();
		if (Input::IsKeyPressed(KD_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * m_Zoom * ts.GetSeconds();
		else if (Input::IsKeyPressed(KD_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * m_Zoom * ts.GetSeconds();

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(KD_KEY_Q))
				m_CameraRotation -= m_CameraRotationSpeed * ts.GetSeconds();
			else if (KDE::Input::IsKeyPressed(KD_KEY_E))
				m_CameraRotation += m_CameraRotationSpeed * ts.GetSeconds();

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		//////////////////////////////////////////////////////////////////////////
		//if (Input::IsMouseButtonPressed(KD_MOUSE_BUTTON_LEFT) && !ImGui::GetIO().WantCaptureMouse)
		//{
		//	if (!m_IsDragging)
		//	{
		//		m_IsDragging = true;
		//
		//		m_TempX = Input::GetMouseX();
		//		m_TempY = Input::GetMouseY();
		//	}
		//	else
		//	{
		//		float m_CurrMouseX = Input::GetMouseX();
		//		float m_CurrMouseY = Input::GetMouseY();
		//
		//		float m_DifferenceX = m_CurrMouseX - m_TempX;
		//		float m_DifferenceY = m_CurrMouseY - m_TempY;
		//
		//		m_CameraPosition.x -= m_DifferenceX * 0.003f * m_Zoom;
		//		m_CameraPosition.y += m_DifferenceY * 0.003f * m_Zoom;
		//
		//		m_TempX = m_CurrMouseX;
		//		m_TempY = m_CurrMouseY;
		//	}
		//
		//	m_Camera.SetPosition(m_CameraPosition);
		//}
		//else
		//{
		//	m_IsDragging = false;
		//}
		//////////////////////////////////////////////////////////////////////////
	}
	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(KD_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(KD_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}
	OrthographicCameraBounds OrthographicCameraController::GetBounds() const
	{
		return m_Bounds;
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_Bounds = { -m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -m_Zoom, m_Zoom };

		m_Zoom -= e.GetOffsetY() * 0.5f;
		if (m_Zoom < m_ZoomLimit) m_Zoom = m_ZoomLimit;

		m_Camera.SetScale(m_Zoom);

		return false;
	}
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_Bounds = { -m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -m_Zoom, m_Zoom };

		//////////////////////////////////////////////////////////////////////////
		//	Screen will be the same size
		//	float yScale = e.GetHeight() / 720.0f;
		//	m_AspectRatio = yScale * (float)e.GetWidth() / e.GetHeight();
		//	m_Camera.SetProjection(-m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -yScale * m_Zoom, yScale * m_Zoom);
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		//	Screen will scale
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -m_Zoom, m_Zoom);

		return false;
	}
}