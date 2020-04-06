#include "stdafx.h"
#include "EventManager.h"

namespace XYZ {
	EventManager EventManager::s_Instance;

	//Add an event to manager
	bool EventManager::FireEvent(event_ptr event)
	{
		auto type = event->GetEventType();

		if (m_Handlers.find(type) == m_Handlers.end())
			return false;

		auto evm_Handlers = m_Handlers[type];

		//Invoke m_Handlers starting at the end of the handler vector, stop once the event becomes handled
		for (auto i = evm_Handlers.rbegin(); !event->IsHandled() && i != evm_Handlers.rend(); ++i)
			(*i)(event);
	}

	//Attach handler to an event
	bool EventManager::AddHandler(EventType type, handlerPtr eventHandler)
	{
		//Initialize a new vector of m_Handlers for the given event name
		if (m_Handlers.find(type) == m_Handlers.end())
			m_Handlers[type] = std::vector<handlerPtr>();

		m_Handlers[type].push_back(eventHandler);
		return true;
	}

	//Remove handler from an event (removal by pointer isn't the best option, but it should be sufficient here)
	bool EventManager::RemoveHandler(EventType type, handlerPtr eventHandler)
	{
		if (m_Handlers.find(type) == m_Handlers.end())
			return false;

		auto ehAddr = getAddress(eventHandler);
		auto i = m_Handlers[type].begin();
		for (; i != m_Handlers[type].end(); ++i)
		{
			auto iAddr = getAddress(*i);
			if (iAddr == ehAddr)
				break;
		}

		if (i == m_Handlers[type].end())
			return false;

		m_Handlers[type].erase(i);
		return true;
	}

	//Removes all m_Handlers from an event
	void EventManager::RemoveAllHandlers(EventType type)
	{
		if (m_Handlers.find(type) == m_Handlers.end())
			m_Handlers[type] = std::vector<handlerPtr>();
		else
			m_Handlers[type].clear();
	}

}