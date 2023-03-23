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

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Camera.SetProjection(-m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -m_Zoom, m_Zoom);
	}
	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		//////////////////////////////////////////////////////////////////////////
		if (Input::IsKeyPressed(Key::W))
		{
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(Key::D))
		{
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}

		if (Input::IsKeyPressed(Key::W))
		{
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(Key::S))
		{
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(Key::Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			if (Input::IsKeyPressed(Key::E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_Zoom * 3.0f;
		//////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////
		//if (Input::IsMouseButtonPressed(Mouse::ButtonLeft) && !ImGui::GetIO().WantCaptureMouse)
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
		//		m_CameraPosition.x -= m_DifferenceX * m_CameraTranslationSpeed;
		//		m_CameraPosition.y += m_DifferenceY * m_CameraTranslationSpeed;
		//
		//		m_TempX = m_CurrMouseX;
		//		m_TempY = m_CurrMouseY;
		//	}
		//
		//	m_Camera.SetPosition(m_CameraPosition);
		//
		//	m_CameraTranslationSpeed = m_Zoom * 0.25f;
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

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_Zoom -= e.GetOffsetY() * 0.25f;
		m_Zoom = std::max(m_Zoom, 0.25f);
		m_Zoom = std::min(m_Zoom, 4.0f);
		m_Camera.SetProjection(-m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -m_Zoom, m_Zoom);

		m_Bounds = { -m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -m_Zoom, m_Zoom };

		m_Camera.SetPosition(m_CameraPosition);

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
		OnResize((float)e.GetWidth(), (float)e.GetHeight());

		return false;
	}
}