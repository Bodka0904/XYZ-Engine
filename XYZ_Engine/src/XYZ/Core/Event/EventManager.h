#pragma once
#include "Event.h"
#include <map>
#include <functional>

namespace XYZ {
	using HandlerID = unsigned int;
	typedef std::function<void(event_ptr)> handlerPtr;
	typedef std::map<EventType, std::vector<std::pair<unsigned int, handlerPtr>>> handler_map;

	class EventManager
	{
	public:
		EventManager(EventManager& other) = delete;

		~EventManager()
		{}

		bool FireEvent(event_ptr event);
		HandlerID AddHandler(EventType type, handlerPtr eventHandler);
		bool RemoveHandler(EventType type, HandlerID handlerId);
		void RemoveAllHandlers(EventType type);

		static EventManager& Get() { return s_Instance; };
	private:
		EventManager() : m_Handlers(handler_map()), m_NextId(0)
		{}

	private:

		handler_map m_Handlers;
		unsigned int m_NextId;

		static EventManager s_Instance;

	};

}
