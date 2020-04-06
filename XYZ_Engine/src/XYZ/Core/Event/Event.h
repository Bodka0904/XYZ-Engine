#pragma once 
#include "../Log.h"

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
		MouseMoved,

		Custom
	};



	class Event
	{
	public:
		virtual EventType GetEventType() const = 0;
		bool& IsHandled() { return m_Handled; }
		void SetHandled() { m_Handled = true; }

	private:
		bool m_Handled = false;
		EventType m_Type = EventType::None;
	};

	//typedef std::shared_ptr<Event> event_ptr;

	using event_ptr = std::shared_ptr<Event>;


	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int width, int height)
			:m_Width(width), m_Height(height)
		{
			XYZ_LOG_INFO("Window: Resized window width: ", width, " height: ", height);
		}


		virtual EventType GetEventType() const override { return m_Type; }
		virtual std::pair<int, int> GetWinSize() const { return { (int)m_Width, (int)m_Height }; }
		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }

	private:
		int m_Width;
		int m_Height;
		EventType m_Type = EventType::WindowResized;

	};


	class KeyPressedEvent : public Event
	{
	public:
		KeyPressedEvent(int key, int mod)
			:m_Key(key), m_Mod(mod)
		{}
		virtual EventType GetEventType() const override { return m_Type; }
		virtual int GetKey() const { return m_Key; }
		virtual int GetMod() const { return m_Mod; }
	private:
		int m_Key;
		int m_Mod;
		EventType m_Type = EventType::KeyPressed;
	};



	class KeyReleasedEvent : public Event
	{
	public:
		KeyReleasedEvent(int key)
			:m_Key(key)
		{}
		virtual EventType GetEventType() const override { return m_Type; }
		virtual int GetKey() const { return m_Key; }

	private:
		int m_Key;
		EventType m_Type = EventType::KeyReleased;
	};




	class MouseButtonPressEvent : public Event
	{
	public:
		MouseButtonPressEvent(int button)
			:m_Button(button)
		{
		};


		virtual EventType GetEventType() const override { return m_Type; }
		inline int GetButton() const { return m_Button; }

	private:
		int m_Button;
		EventType m_Type = EventType::MouseButtonPressed;
	};


	class MouseButtonReleaseEvent : public Event
	{
	public:
		MouseButtonReleaseEvent(int button)
			:m_Button(button)
		{
		};

		virtual EventType GetEventType() const override { return m_Type; }
		inline int GetButton() const { return m_Button; }

	private:
		int m_Button;
		EventType m_Type = EventType::MouseButtonReleased;
	};

	class MouseScrollEvent :public Event
	{
	public:
		MouseScrollEvent(float xOffset, float yOffset)
			:m_XOffset(xOffset), m_YOffset(yOffset)
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