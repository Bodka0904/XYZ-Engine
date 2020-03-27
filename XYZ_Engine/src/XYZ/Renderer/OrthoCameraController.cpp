#include "stdafx.h"
#include "OrthoCameraController.h"

#include "XYZ/Core/KeyCodes.h"
#include "XYZ/Core/Input.h"

namespace XYZ {
	OrthoCameraController::OrthoCameraController(float aspectRatio, bool rotation)
		: 
		m_AspectRatio(aspectRatio), 
		m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{
	}

	void OrthoCameraController::OnUpdate(float dt)
	{
		if (Input::IsKeyPressed(KeyCode::XYZ_KEY_A))
		{
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
		}
		else if (Input::IsKeyPressed(KeyCode::XYZ_KEY_D))
		{
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
		}

		if (Input::IsKeyPressed(KeyCode::XYZ_KEY_W))
		{
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
		}
		else if (Input::IsKeyPressed(KeyCode::XYZ_KEY_S))
		{
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
		}

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(KeyCode::XYZ_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * dt;
			if (Input::IsKeyPressed(KeyCode::XYZ_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * dt;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthoCameraController::OnEvent(Event& event)
	{
		if (event.GetEventType() == XYZ::EventType::MouseScroll)
		{
			XYZ::MouseScrollEvent& e = (XYZ::MouseScrollEvent&)event;
			OrthoCameraController::OnMouseScrolled(e);
		}
		else if (event.GetEventType() == XYZ::EventType::WindowResized)
		{
			XYZ::WindowResizeEvent& e = (XYZ::WindowResizeEvent&) event;
			OrthoCameraController::OnWindowResized(e);
		}
	}

	bool OrthoCameraController::OnMouseScrolled(MouseScrollEvent& e)
	{
		m_ZoomLevel -= e.GetOffsetY() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthoCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}