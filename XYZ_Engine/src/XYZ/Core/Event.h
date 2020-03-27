#pragma once 
#include "Log.h"

/*
	Very simple event system, requires update in the future.
*/


namespace XYZ {
	enum class EventType
	{
		None = 0,

		WindowClosed,
		WindowResized,

		KeyPressed,
		KeyReleased,

		MouseButtonPressed,
		MouseButtonReleased,

		MouseScroll,
		MouseMoved
	};



	class Event
	{
	public:
		virtual EventType GetEventType() const = 0;
		bool& IsHandled() { return handled; }

	private:
		EventType type = EventType::None;
		bool handled = false;
	};



	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int width, int height)
			:m_width(width), m_height(height)
		{
			XYZ_LOG_INFO("Window: Resized window width: ", width, " height: ", height);
		}


		virtual EventType GetEventType() const override { return type; }
		virtual std::pair<int, int> GetWinSize() const { return { (int)m_width, (int)m_height }; }
		inline int GetWidth() const { return m_width; }
		inline int GetHeight() const { return m_height; }

	private:
		int m_width;
		int m_height;
		EventType type = EventType::WindowResized;

	};


	class KeyPressedEvent : public Event
	{
	public:
		KeyPressedEvent(int key, int mod)
			:m_key(key), m_mod(mod)
		{}
		virtual EventType GetEventType() const override { return type; }
		virtual int GetKey() const { return m_key; }
		virtual int GetMod() const { return m_mod; }
	private:
		int m_key;
		int m_mod;
		EventType type = EventType::KeyPressed;
	};



	class KeyReleasedEvent : public Event
	{
	public:
		KeyReleasedEvent(int key)
			:m_key(key)
		{}
		virtual EventType GetEventType() const override { return type; }
		virtual int GetKey() const { return m_key; }

	private:
		int m_key;
		EventType type = EventType::KeyReleased;
	};




	class MouseButtonPressEvent : public Event
	{
	public:
		MouseButtonPressEvent(int button)
			:m_button(button)
		{
		};


		virtual EventType GetEventType() const override { return type; }
		inline int GetButton() const { return m_button; }

	private:
		int m_button;
		EventType type = EventType::MouseButtonPressed;
	};




	class MouseButtonReleaseEvent : public Event
	{
	public:
		MouseButtonReleaseEvent(int button)
			:m_button(button)
		{
		};

		virtual EventType GetEventType() const override { return type; }
		inline int GetButton() const { return m_button; }

	private:
		int m_button;
		EventType type = EventType::MouseButtonReleased;
	};

	class MouseScrollEvent :public Event
	{
	public:
		MouseScrollEvent(float xOffset,float yOffset)
			:m_XOffset(xOffset),m_YOffset(yOffset)
		{};

		virtual EventType GetEventType() const override { return type; }
		inline double GetOffsetX() const { return m_XOffset; }
		inline double GetOffsetY() const { return m_YOffset; }
	private:
		EventType type = EventType::MouseScroll;
		float m_XOffset;
		float m_YOffset;
	};

}