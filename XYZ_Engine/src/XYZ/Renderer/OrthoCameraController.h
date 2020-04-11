#pragma once

#include "XYZ/Renderer/OrthoCamera.h"
#include "XYZ/Core/Event/EventManager.h"


namespace XYZ {
	struct OrthoCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};
	class OrthoCameraController
	{
	public:
		OrthoCameraController(float aspectRatio, bool rotation = false);
		~OrthoCameraController();

		void OnUpdate(float dt);

		OrthoCamera& GetCamera() { return m_Camera; }
		const OrthoCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }

		const OrthoCameraBounds& GetBounds() const { return m_Bounds; }

	private:
		void OnMouseScrolled(event_ptr event);
		void OnWindowResized(event_ptr event);
	private:
		HandlerID m_MouseScroll;
		HandlerID m_WindowResize;

		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthoCamera m_Camera;
		OrthoCameraBounds m_Bounds;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f; //In degrees, in the anti-clockwise direction
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
	};

}